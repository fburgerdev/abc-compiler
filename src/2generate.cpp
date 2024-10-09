#include "semantics/source.hpp"
#include "generate/code.hpp"

namespace Compiler {
    namespace CPP {
        // static
        static Map<const Compiler::Var*, Var*> s_varMap;
        static Map<const Compiler::Func*, Func*> s_funcMap;
        static Map<const Compiler::Class*, Struct*> s_classMap;
        // spell
        static string spell(const string& str) {
            static Map<string, string> map({
                { "++", "inc" }, { "--", "dec" },
                { "==", "eq" },
                { "!=", "neq" },
                { "<", "lt" },
                { "<=", "lte" },
                { ">", "gt" },
                { ">=", "gte" },
                { "=", "assign" },
                { "+=", "addassign" },
                { "-=", "subassign" },
                { "*=", "mulassign" },
                { "/=", "divassign" },
                { "%=", "modassign" },
                { "+", "add" },
                { "-", "sub" },
                { "*", "mul" },
                { "/", "div" },
                { "%", "mod" },
            });
            if (map.contains(str)) {
                return map.at(str);
            }
            return str;
        }
        static uint64 s_count = 0;
        static Map<const void*, uint64> s_map;
        string getName(const void* ptr) {
            if (!s_map.contains(ptr)) {
                s_map.emplace(ptr, ++s_count);
            }
            return "_" + std::to_string(s_map.at(ptr));
        }
        string getName(const Compiler::Class* ptr) {
            return spell(string(ptr->name)) + getName((const void*)ptr);
        }
        string getName(const Compiler::Func* ptr) {
            return spell(string(ptr->name)) + getName((const void*)ptr);
        }
        string getName(const Compiler::Var* ptr) {
            return spell(string(ptr->name)) + getName((const void*)ptr);
        }

        // Type
        template<>
        Type::Type<Compiler::Type>(const Compiler::Type& type) {
            if (type.qualifier == Compiler::Qualifier::CONST) {
                qualifier = Qualifier::CONST;
            }

            if (std::holds_alternative<Compiler::Primitive>(type.value)) {
                switch (std::get<Compiler::Primitive>(type.value)) {
                case Compiler::Primitive::BOOL:
                    value = Primitive::BOOL;
                    break;
                case Compiler::Primitive::CHAR:
                    value = Primitive::CHAR;
                    break;
                case Compiler::Primitive::INT:
                    value = Primitive::I32;
                    break;
                case Compiler::Primitive::UINT:
                    value = Primitive::U32;
                    break;
                case Compiler::Primitive::FLOAT:
                    value = Primitive::F32;
                    break;
                default:
                    break;
                }
            }
            else if (std::holds_alternative<Compiler::Class*>(type.value)) {
                value = getName(std::get<Compiler::Class*>(type.value));
            }
            else if (std::holds_alternative<ID>(type.value)) {
                value = string(std::get<ID>(type.value).name()); //TODO: remove
            }

            if (type.category == Compiler::ValueCategory::LVALUE) {
                category = Category::LVALUE;
            }
            else if (type.category == Compiler::ValueCategory::RVALUE) {
                category = Category::RVALUE;
            }
        }
        // Var 
        template<>
        Var::Var<Compiler::Var>(const Compiler::Var& var) {
            s_varMap.emplace(&var, this);
            if (var.specifier == Compiler::Specifier::LOCAL) {
                storage = Storage::STATIC;
            }
            name = getName(&var);
            type = Type(std::get<Compiler::Type>(var.type));
        }
        // Func
        template<>
        Func::Func<Compiler::Func>(const Compiler::Func& func) {
            s_funcMap.emplace(&func, this);
            if (func.specifier == Compiler::Specifier::LOCAL) {
                storage = Storage::STATIC;
            }
            name = getName(&func);
            if (func.name == "init") {
                params.emplace_back();
                params.back().storage = Storage::NONE;
                params.back().name = "__self";
                params.back().type.category = Category::LVALUE;
                if (func.qualifier == Compiler::Qualifier::CONST) {
                    params.back().type.qualifier = Qualifier::CONST;
                }
                params.back().type.value = getName(dynamic_cast<const Compiler::Class*>(func._parent));
            }
            for (const auto& param : func.params) {
                params.emplace_back(param);
            }
            if (func.returnType) {
                returnType = Type(func.returnType.value());
            }
        }
        // Struct
        template<>
        Struct::Struct<Compiler::Class>(const Compiler::Class& cls) {
            s_classMap.emplace(&cls, this);
            name = getName(&cls);
            for (const auto& [name, content] : cls.vars) {
                if (content.var.specifier == Compiler::Specifier::NONE) {
                    members.emplace_back(content.var);
                }
            }
        }

        // Header
        template<>
        void Header::add<Compiler::Class>(const Compiler::Class& cls) {
            structs.emplace_back(cls);
            for (const auto& [name, content] : cls.vars) {
                if (content.var.specifier == Compiler::Specifier::STATIC) {
                    vars.emplace_back(content.var);
                }
            }
            for (const auto& [name, content] : cls.funcs) {
                if (content.func.specifier == Compiler::Specifier::NONE) {
                    funcs.emplace_back(content.func);
                    //TODO: self
                }
                else if (content.func.specifier == Compiler::Specifier::STATIC) {
                    funcs.emplace_back(content.func);
                }
            }
        }
        template<>
        void Header::add<Compiler::Group>(const Compiler::Group& grp) {
            for (const auto& [name, child] : grp.groups) {
                add(child);
            }
            for (const auto& [name, child] : grp.classes) {
                add(child);
            }
            for (const auto& [name, child] : grp.funcs) {
                if (child.specifier == Compiler::Specifier::LOCAL) {
                    continue;
                }
                funcs.emplace_back(child);
            }
            for (const auto& [name, child] : grp.vars) {
                if (child.specifier == Compiler::Specifier::LOCAL) {
                    continue;
                }
                vars.emplace_back(child);
            }
        }
        template<>
        Header::Header<Compiler::Group>(const Compiler::Group& grp) {
            add(grp);
        }

        template<>
        Expr::Expr<Compiler::Expr>(const Compiler::Expr& expr);
        // Literal
        template<>
        Literal::Literal<Compiler::Literal>(const Compiler::Literal& lit) {
            value = lit.value;
        }
        // VarAccess
        template<>
        VarAccess::VarAccess<Compiler::VarAccess>(const Compiler::VarAccess& access) {
            var = s_varMap.at(access.boundVar);
        }
        // MemberAccess
        template<>
        MemberAccess::MemberAccess<Compiler::MemberAccess>(const Compiler::MemberAccess& access) {
            expr = makeUniquePtr<Expr>(*(access.expr));
            member = string(access.member);
        }
        // Call
        template<>
        Call::Call<Compiler::StaticCall>(const Compiler::StaticCall& call) {
            func = s_funcMap.at(call.boundFunc);
            for (const auto& arg : call.args) {
                args.emplace_back(arg);
            }
        }
        template<>
        Call::Call<Compiler::MemberCall>(const Compiler::MemberCall& call) {
            func = s_funcMap.at(call.boundFunc);
            args.emplace_back(*(call.expr));
            for (const auto& arg : call.args) {
                args.emplace_back(arg);
            }
        }
        // Expr
        template<>
        Expr::Expr<Compiler::Expr>(const Compiler::Expr& expr) {
            if (std::holds_alternative<Compiler::Literal>(expr.value)) {
                value = Literal(std::get<Compiler::Literal>(expr.value));
            }
            else if (std::holds_alternative<Compiler::VarAccess>(expr.value)) {
                value = VarAccess(std::get<Compiler::VarAccess>(expr.value));
            }
            else if (std::holds_alternative<Compiler::MemberAccess>(expr.value)) {
                value = MemberAccess(std::get<Compiler::MemberAccess>(expr.value));
            }
            else if (std::holds_alternative<Compiler::StaticCall>(expr.value)) {
                value = Call(std::get<Compiler::StaticCall>(expr.value));
            }
            else if (std::holds_alternative<Compiler::MemberCall>(expr.value)) {
                value = Call(std::get<Compiler::MemberCall>(expr.value));
            }
        }

        // CodeBlock
        template<>
        CodeBlock::CodeBlock<Compiler::CodeBlock>(const Compiler::CodeBlock& block) {
            for (const auto& stmt : block.stmts) {
                stmts.emplace_back(stmt);
            }
        }

        // LocalDecl
        template<>
        LocalDecl::LocalDecl<Compiler::LocalDecl>(const Compiler::LocalDecl& stmt) {
            var = Var(stmt.var);
            expr = Expr(stmt.expr);
        }
        // Cond
        template<>
        Cond::Cond<Compiler::Cond>(const Compiler::Cond& stmt) {
            ifCond = Pair<Expr, CodeBlock>(Expr(stmt.ifCond.first), CodeBlock(stmt.ifCond.second));
            for (const auto& [elifCond, elifBlock] : stmt.elifConds) {
                elifConds.emplace_back(elifCond, elifBlock);
            }
            if (stmt.elseBlock) {
                elseBlock = CodeBlock(stmt.elseBlock.value());
            }
        }
        // ForLoop
        template<>
        ForLoop::ForLoop<Compiler::ForLoop>(const Compiler::ForLoop& stmt) {
            //TODO: implement
            block = CodeBlock((const Compiler::CodeBlock&)stmt);
        }
        template<>
        ForLoop::ForLoop<Compiler::WhileLoop>(const Compiler::WhileLoop& stmt) {
            cond = Expr(stmt.expr);
            block = CodeBlock((const Compiler::CodeBlock&)stmt);
        }
        // Return
        template<>
        Return::Return<Compiler::Return>(const Compiler::Return& stmt) {
            if (stmt.expr) {
                expr = Expr(stmt.expr.value());
            }
        }

        // Stmt
        template<>
        Stmt::Stmt<Compiler::Stmt>(const Compiler::Stmt& stmt) {
            if (std::holds_alternative<Compiler::LocalDecl>(stmt.value)) {
                value = LocalDecl(std::get<Compiler::LocalDecl>(stmt.value));
            }
            else if (std::holds_alternative<Compiler::Expr>(stmt.value)) {
                value = Expr(std::get<Compiler::Expr>(stmt.value));
            }
            else if (std::holds_alternative<Compiler::Cond>(stmt.value)) {
                value = Cond(std::get<Compiler::Cond>(stmt.value));
            }
            else if (std::holds_alternative<Compiler::ForLoop>(stmt.value)) {
                value = ForLoop(std::get<Compiler::ForLoop>(stmt.value));
            }
            else if (std::holds_alternative<Compiler::WhileLoop>(stmt.value)) {
                value = ForLoop(std::get<Compiler::WhileLoop>(stmt.value));
            }
            else if (std::holds_alternative<Compiler::Skip>(stmt.value)) {
                value = Continue();
            }
            else if (std::holds_alternative<Compiler::Break>(stmt.value)) {
                value = Break();
            }
            else if (std::holds_alternative<Compiler::Return>(stmt.value)) {
                value = Return(std::get<Compiler::Return>(stmt.value));
            }
        }
   }
}