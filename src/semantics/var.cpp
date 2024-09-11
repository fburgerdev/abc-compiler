#include "var.hpp"

namespace Compiler {
    // Var
    bool Var::dependsOn(const Class& cls) const {
        return getType().dependsOn(cls);
    }

    // StaticVar
    Type StaticVar::getType() const {
        return std::get<Type>(type);
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
        return std::get<Type>(type);
    }
}