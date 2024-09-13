#include "var.hpp"

namespace Compiler {
    // Var
    bool Var::dependsOn(const Class& cls) const {
        return getType().dependsOn(cls);
    }

    // StaticVar
    Type StaticVar::getType() const {
        if (std::holds_alternative<Type>(type)) {
            return std::get<Type>(type);
        }
        return Type(Identifier());
    }
    // MemberVar
    Type MemberVar::getType() const {
        return type;
    }
    // FuncVar
    Type FuncVar::getType() const {
        return type;
    }
    // LocalVar
    Type LocalVar::getType() const {
        if (std::holds_alternative<Type>(type)) {
            return std::get<Type>(type);
        }
        return Type(Identifier());
    }
}