#include "func.hpp"

namespace Compiler {
    // setScope
    void Func::setScope(Scope* scope) {
        parentScope = scope;
        for (auto& param : params) {
            param.scope = this;
        }
    }
    // searchClass
    Class* Func::searchClass(const Identifier& id) {
        return nullptr; //TODO: implement
    }
    // searchFunc
    Func* Func::searchFunc(const Identifier& id, const List<Type>& argTypes) {
        return nullptr; //TODO: implement
    }
    // searchVar
    Var* Func::searchVar(const Identifier& id) {
        return nullptr; //TODO: implement
    }

    // isOperator
    bool Func::isOperator() const {
        return !std::isalpha(name.front());
    }
    // matches
    bool Func::matches(const List<Type>& argTypes) {
        if (params.size() != argTypes.size()) {
            return false;
        }
        for (uint i = 0; i < params.size(); ++i) {
            if (params[i].type != argTypes[i]) {
                return false;
            }
        }
        return true;
    }
    // dependsOn
    bool Func::dependsOn(const Class& cls) const {
        bool depends = false;
        for (const auto& param : params) {
            depends |= param.type.dependsOn(cls);
        }
        depends |= returnType.dependsOn(cls);
    }
}