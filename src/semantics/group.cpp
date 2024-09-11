#include "group.hpp"

namespace Compiler {
    // setScope
    void Group::setScope(Scope* scope) {
        parentScope = scope;
        for (auto& [name, group] : groups) {
            group.setScope(this);
        }
        for (auto& [name, cls] : classes) {
            cls.setScope(this);
        }
        for (auto& [name, func_list] : funcs) {
            for (auto& func : func_list) {
                func.setScope(this);
            }
        }
        for (auto& [name, var] : vars) {
            var.scope = this;
        }
    }
    // searchClass
    Class* Group::searchClass(const Identifier& id) {
        return nullptr; //TODO: implement
    }
    // searchFunc
    Func* Group::searchFunc(const Identifier& id, const List<Type>& argTypes) {
        return nullptr; //TODO: implement
    }
    // searchVar
    Var* Group::searchVar(const Identifier& id) {
        return nullptr; //TODO: implement
    }
}