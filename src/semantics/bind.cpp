#include "bind.hpp"

namespace Compiler {
    // bindTypes
    // :: var
    void bindTypes(Var& var, Handler& handler) {
        if (std::holds_alternative<Type>(var.type)) {
            Type& type = std::get<Type>(var.type);
            if (std::holds_alternative<ID>(type.value)) {
                if (Class* found = var.context->findClass(std::get<ID>(type.value))) {
                    type.value = found;
                }
                else {
                    //TODO: handler
                }
            }
        }
    }
    // :: func
    void bindTypes(Func& func, Handler& handler) {
        for (auto& param : func.params) {
            bindTypes(param, handler);
        }
        if (func.returnType) {
            Type& type = func.returnType.value();
            if (std::holds_alternative<ID>(type.value)) {
                if (Class* found = func.findClass(std::get<ID>(type.value))) {
                    type.value = found;
                }
                else {
                    //TODO: handler
                }
            }
        }
    }
    // :: class
    void bindTypes(Class& cls, Handler& handler) {
        for (auto& [name, content] : cls.classes) {
            bindTypes(content.cls, handler);
        }
        for (auto& [name, content] : cls.funcs) {
            bindTypes(content.func, handler);
        }
        for (auto& [name, content] : cls.vars) {
            bindTypes(content.var, handler);
        }
    }
    // :: group
    void bindTypes(Group& grp, Handler& handler) {
        for (auto& [name, child] : grp.groups) {
            bindTypes(child, handler);
        }
        for (auto& [name, child] : grp.classes) {
            bindTypes(child, handler);
        }
        for (auto& [name, child] : grp.funcs) {
            bindTypes(child, handler);
        }
        for (auto& [name, child] : grp.vars) {
            bindTypes(child, handler);
        }
    }

    // bindExprs
    // :: exprs
    void bindExprs(
        [[maybe_unused]] Literal& literal,
        [[maybe_unused]] Context& context,
        [[maybe_unused]] Handler& handler) {
        return;
    }
    void bindExprs(VarAccess& varAccess, Context& context, Handler& handler) {
        if (Var* var = context.findVar(varAccess.id)) {
            varAccess.boundVar = var;
        }
        else {
            //TODO: handler
        }
    }
    void bindExprs(MemberAccess& memberAccess, Context& context, Handler& handler) {
        bindExprs(*(memberAccess.expr), context, handler);
        if (!handler.shouldCancel()) {
            auto value = memberAccess.expr->getType().value;
            if (std::holds_alternative<Class*>(value)) {
                Class& cls = *(std::get<Class*>(value));
                if (cls.vars.contains(string(memberAccess.member))) {
                    auto& [access, var] = cls.vars.at(string(memberAccess.member));
                    //TODO: check for invalid private access
                    if (var.specifier == Specifier::STATIC) {
                        //TODO: handler
                    }
                    memberAccess.boundVar = &var;
                }
            }
            else {
                //TODO: handler
            }
        }
    }
    void bindExprs(StaticCall& staticCall, Context& context, Handler& handler) {
        if (Func* func = context.findFunc(staticCall.id)) {
            staticCall.boundFunc = func;
        }
        else {
            //TODO: handler
        }
    }
    void bindExprs(MemberCall& memberCall, Context& context, Handler& handler) {
        bindExprs(*(memberCall.expr), context, handler);
        if (!handler.shouldCancel()) {
            auto value = memberCall.expr->getType().value;
            if (std::holds_alternative<Class*>(value)) {
                Class& cls = *(std::get<Class*>(value));
                if (cls.funcs.contains(string(memberCall.member))) {
                    auto& [access, func] = cls.funcs.at(string(memberCall.member));
                    //TODO: check for invalid private access
                    if (func.specifier == Specifier::STATIC) {
                        //TODO: handler
                    }
                    memberCall.boundFunc = &func;
                }
            }
            else {
                //TODO: handler
            }
        }
    }
    void bindExprs(Expr& expr, Context& context, Handler& handler) {
        std::visit([&](auto& value) {
            return bindExprs(value, context, handler);
        }, expr.value);
    }
    // :: stmts
    void bindExprs(LocalDecl& localDecl, Context& context, Handler& handler) {
        bindExprs(localDecl.expr, context, handler);
        if (!handler.shouldCancel()) {
            localDecl.var.type = localDecl.expr.getType();
            //? how to handle value categories
        }
    }
    void bindExprs(Cond& cond, Context& context, Handler& handler) {
        bindExprs(cond.ifCond.first, context, handler);
        bindExprs(cond.ifCond.second, context, handler);
        for (auto& [elifCond, elifBlock] : cond.elifConds) {
            bindExprs(elifCond, context, handler);
            bindExprs(elifBlock, context, handler);
        }
        if (cond.elseBlock) {
            bindExprs(cond.elseBlock.value(), context, handler);
        }
    }
    void bindExprs(ForLoop& forLoop, Context& context, Handler& handler) {
        bindExprs(forLoop.expr, context, handler);
        //TODO: bind var
        if (!handler.shouldCancel()) {
            bindExprs((CodeBlock&)forLoop, context, handler);
        }
    }
    void bindExprs(WhileLoop& whileLoop,Context& context, Handler& handler) {
        bindExprs(whileLoop.expr, context, handler);
        if (!handler.shouldCancel()) {
            bindExprs((CodeBlock&)whileLoop, context, handler);
        }
    }
    void bindExprs(
        [[maybe_unused]] Skip& skip_,
        [[maybe_unused]] Context& context,
        [[maybe_unused]] Handler& handler) {
        return;
    }
    void bindExprs(
        [[maybe_unused]] Break& break_,
        [[maybe_unused]] Context& context,
        [[maybe_unused]] Handler& handler) {
        return;
    }
    void bindExprs(Return& return_, Context& context, Handler& handler) {
        if (return_.expr) {
            bindExprs(return_.expr.value(), context, handler);
        }
    }
    void bindExprs(Stmt& stmt, Context& context, Handler& handler) {
        std::visit([&](auto& value) {
            return bindExprs(value, context, handler);
        }, stmt.value);
    }
    // :: block
    void bindExprs(CodeBlock& codeBlock, Context& context, Handler& handler) {
        for (auto& stmt : codeBlock.stmts) {
            bindExprs(stmt, codeBlock, handler);
        }
    }
}