#include "expr.hpp"

namespace Compiler {
    namespace ABC {
        // Literal
        Type Literal::getType() const {
            Type type;
            type.category = ValueCategory::RVALUE;
            switch (litType) {
            case LiteralType::BOOL:
                type.value = Primitive::BOOL;
                break;
            case LiteralType::INT:
                type.value = Primitive::INT;
                break;
            case LiteralType::FLOAT:
                type.value = Primitive::FLOAT;
                break;
            case LiteralType::CHAR:
                type.value = Primitive::CHAR;
                break;
            }
            return type;
        }
        // VarAccess
        Type VarAccess::getType() const {
            Type type = std::get<Type>(boundVar->type);
            type.category = ValueCategory::LVALUE;
            return type;
        }
        // MemberAccess
        Type MemberAccess::getType() const {
            Type type = std::get<Type>(boundVar->type);
            type.category = ValueCategory::LVALUE;
            return type;
        }
        // StaticCall
        Type StaticCall::getType() const {
            Type type = boundFunc->returnType.value();
            if (type.category == ValueCategory::NONE) {
                type.category = ValueCategory::RVALUE;
            }
            return type;
        }
        // MemberCall
        Type MemberCall::getType() const {
            Type type = boundFunc->returnType.value();
            if (type.category == ValueCategory::NONE) {
                type.category = ValueCategory::RVALUE;
            }
            return type;
        }
        // Expr
        Type Expr::getType() const {
            return std::visit([](const auto& value) {
                return value.getType();
            }, value);
        }
    }
}