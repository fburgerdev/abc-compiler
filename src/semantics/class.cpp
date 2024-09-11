#include "class.hpp"

namespace Compiler {
    // setScope
    void Class::setScope(Scope* scope) {
        parentScope = scope;
        for (auto& [access, func] : initFuncs) {
            func.setScope(this);
        }
        for (auto& [access, func] : memberFuncs) {
            func.setScope(this);
        }
        for (auto& [access, var] : memberVars) {
            var.scope = this;
        }
        for (auto& [access, func] : staticFuncs) {
            func.setScope(this);
        }
        for (auto& [access, var] : staticVars) {
            var.scope = this;
        }
        for (auto& [access, cls] : classes) {
            cls.setScope(this);
        }
    }
    // searchClass
    Class* Class::searchClass(const Identifier& id) {
        return nullptr; //TODO: implement
    }
    // searchFunc
    Func* Class::searchFunc(const Identifier& id, const List<Type>& argTypes) {
        return nullptr; //TODO: implement
    }
    // searchVar
    Var* Class::searchVar(const Identifier& id) {
        return nullptr; //TODO: implement
    }

    // dependsOn
    bool Class::dependsOn(const Class& cls) const {
        for (const auto& [access, func] : initFuncs) {
            if (func.dependsOn(cls)) {
                return true;
            }
        }
        for (const auto& [access, func] : memberFuncs) {
            if (func.dependsOn(cls)) {
                return true;
            }
        }
        for (const auto& [access, var] : memberVars) {
            if (var.dependsOn(cls)) {
                return true;
            }
        }
        for (const auto& [access, func] : staticFuncs) {
            if (func.dependsOn(cls)) {
                return true;
            }
        }
        for (const auto& [access, var] : staticVars) {
            if (var.dependsOn(cls)) {
                return true;
            }
        }
        for (const auto& [access, cls] : classes) {
            if (cls.dependsOn(cls)) {
                return true;
            }
        }
        return false;
    }
}