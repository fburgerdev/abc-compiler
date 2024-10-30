#include "generator_c.hpp"

namespace Compiler {
    // constructor
    GeneratorC::GeneratorC(ostream& stream)
        : _stream(stream) {}

    // type
    void GeneratorC::generate(IR::Primitive primitive) {
        switch (primitive) {
        case IR::Primitive::VOID:
            _stream << "void";
            break;
        case IR::Primitive::BOOL:
            _stream << "bool";
            break;
        case IR::Primitive::CHAR_UTF8:
            _stream << "char8";
            break;
        case IR::Primitive::INT8:
            _stream << "int8";
            break;
        case IR::Primitive::INT16:
            _stream << "int16";
            break;
        case IR::Primitive::INT32:
            _stream << "int32";
            break;
        case IR::Primitive::INT64:
            _stream << "int64";
            break;
        case IR::Primitive::UINT8:
            _stream << "uint8";
            break;
        case IR::Primitive::UINT16:
            _stream << "uint16";
            break;
        case IR::Primitive::UINT32:
            _stream << "uint32";
            break;
        case IR::Primitive::UINT64:
            _stream << "uint64";
            break;
        case IR::Primitive::FLOAT32:
            _stream << "float32";
            break;
        case IR::Primitive::FLOAT64:
            _stream << "float64";
            break;
        default:
            break;
        }
    }
    void GeneratorC::generate(const IR::Type& type) {
        // qualifier
        if (type.qualifier == IR::Qualifier::MUT) {
            // pass
        }
        else if (type.qualifier == IR::Qualifier::CONST) {
            _stream << "const" << ' ';
        }
        // value
        if (holds<IR::Primitive>(type.value)) {
            generate(std::get<IR::Primitive>(type.value));
        }
        else if (holds<IR::ProductType>(type.value)) {
            _stream << _table[&std::get<IR::ProductType>(type.value)];
        }
        else if (holds<IR::SumType>(type.value)) {
            _stream << _table[&std::get<IR::SumType>(type.value)];
        }
        generate(type.value);
        // category
        if (type.category == IR::Category::VAL) {
            // pass
        }
        else if (type.category == IR::Category::REF) {
            _stream << '*';
        }
    }
    // expr
    void GeneratorC::generate(const IR::Literal& literal) {
        if (holds<bool>(literal.value)) {
            _stream << (std::get<bool>(literal.value) ? "true" : "false");
        }
        //TODO
    }
    void GeneratorC::generate(const IR::VarAccess& varAccess) {
        _stream << _table[varAccess.bound];
        const IR::Type* prev = varAccess.bound;
        for (const IR::Type* member : varAccess.members) {
            if (prev->category == IR::Category::VAL) {
                _stream << '.';
            }
            else if (prev->category == IR::Category::REF) {
                _stream << "->";
            }
            _stream << _table[prev];
            prev = member;
        }
    }
    void GeneratorC::generate(const IR::FuncCall& funcCall) {
        _stream << _table[funcCall.bound] << '(';
        for (const IR::Expr& expr : funcCall.args) {
            generate(expr);
            _stream << ',';
        }
        _stream << ')';
    }
    void GeneratorC::generate(const IR::Expr& expr) {
        generate(expr.value);
    }
    // stmt
    void GeneratorC::generate(const IR::LocalDecl& localDecl) {
        generate(localDecl.type);
        _stream << ' ' << _table[&localDecl.type];
        if (localDecl.init) {
            _stream << '=';
            generate(localDecl.init.value());
        }
        _stream << ';';
    }
    void GeneratorC::generate(const IR::Cond& cond) {
        // if
        const auto& [ifExpr, ifBlock] = cond.ifCond;
        _stream << "if" << '(';
        generate(ifExpr);
        _stream << ')';
        generate(ifBlock);
        // elif
        for (const auto& [expr, block] : cond.elifConds) {
            _stream << "else if" << '(';
            generate(expr);
            _stream << ')';
            generate(block);
        }
        // else
        if (cond.elseBlock) {
            _stream << "else";
            generate(cond.elseBlock.value());
        }
    }
    void GeneratorC::generate(const IR::Loop& loop) {
        _stream << "while" << '(';
        generate(loop.expr);
        _stream << ')';
        generate(loop.block);
    }
    void GeneratorC::generate(const IR::Stmt& stmt) {
        generate(stmt.value);
    }
    void GeneratorC::generate(const IR::CodeBlock& block) {
        _stream << '{';
        for (const IR::Stmt& stmt : block.stmts) {
            generate(stmt);
        }
        _stream << '}';
    }
    // ProductType
    void GeneratorC::generate(const IR::ProductType& product) {
        _stream << "struct" << ' ' << _table[&product] << '{';
        for (const IR::Type& subtype : product.types) {
            generate(subtype);
            _stream << ' ' << _table[&subtype] << ';';
        }
        _stream << '}' << ';';
    }
    // SumType
    void GeneratorC::generate(const IR::SumType& sum) {
        _stream << "union" << ' ' << _table[&sum] << '{';
        for (const IR::Type& subtype : sum.types) {
            generate(subtype);
            _stream << ' ' << _table[&subtype] << ';';
        }
        _stream << '}' << ';';
    }
    // func
    void GeneratorC::generate(const IR::Func& funcHead) {
        generate(funcHead.returnType);
        _stream << _table[&funcHead] << '(';
        for (const IR::Type& param : funcHead.params) {
            generate(param);
            _stream << ',';
        }
        _stream << ')';
    }
    // source
    void GeneratorC::generate(const IR::Source& source) {
        // structs / unions
        auto structs = source.getProductTypes();
        for (auto& struct_ : structs) {
            generate(struct_);
        } 
        auto unions = source.getSumTypes();
        for (auto& union_ : unions) {
            generate(union_);
        }
        // funcs
        for (const auto& [func, _] : source.funcs) {
            generate(func);
            _stream << ';';
        }
        // code
        for (const auto& [func, body] : source.funcs) {
            generate(func);
            generate(body);
        }
    }
}