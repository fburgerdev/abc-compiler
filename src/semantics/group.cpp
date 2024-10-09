#include "group.hpp"

namespace Compiler {
    // findInCurrent
    Var* Group::findVarInCurrent(ID id, uint depth) {
        if (depth == 0 && id.length() && id.context(0).empty()) {
            if (isGlobal()) {
                return findVarInCurrent(id, 1);
            }
            return nullptr;
        }

        if (id.length() == depth) {
            string name(id.name());
            if (vars.contains(name)) {
                return &vars.at(name);
            }
            return nullptr;
        }
        string name(id.context(depth));
        if (groups.contains(name)) {
            return groups.at(name).findVarInCurrent(id, depth + 1);
        }
        if (classes.contains(name)) {
            return classes.at(name).findVarInCurrent(id, depth + 1);
        }
        return nullptr;
    }
    Func* Group::findFuncInCurrent(ID id, uint depth) {
        if (depth == 0 && id.length() && id.context(0).empty()) {
            if (isGlobal()) {
                return findFuncInCurrent(id, 1);
            }
            return nullptr;
        }

        if (id.length() == depth) {
            string name(id.name());
            if (funcs.contains(name)) {
                return &funcs.at(name);
            }
            return nullptr;
        }
        string name(id.context(depth));
        if (groups.contains(name)) {
            return groups.at(name).findFuncInCurrent(id, depth + 1);
        }
        if (classes.contains(name)) {
            return classes.at(name).findFuncInCurrent(id, depth + 1);
        }
        return nullptr;
    }
    Class* Group::findClassInCurrent(ID id, uint depth) {
        if (depth == 0 && id.length() && id.context(0).empty()) {
            if (isGlobal()) {
                return findClassInCurrent(id, 1);
            }
            return nullptr;
        }

        if (id.length() == depth) {
            string name(id.name());
            if (classes.contains(name)) {
                return &classes.at(name);
            }
            return nullptr;
        }
        string name(id.context(depth));
        if (groups.contains(name)) {
            return groups.at(name).findClassInCurrent(id, depth + 1);
        }
        if (classes.contains(name)) {
            return classes.at(name).findClassInCurrent(id, depth + 1);
        }

        return nullptr;
    }
}