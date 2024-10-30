#include "class.hpp"

namespace Compiler {
    namespace ABC {
        // findInCurrent
        Var* Class::findVarInCurrent(ID id, uint depth) {
            if (id.length() == depth) {
                string name(id.name());
                if (vars.contains(name)) {
                    return &vars.at(name).var;
                }
                return nullptr;
            }
            string name(id.context(depth));
            if (classes.contains(name)) {
                return classes.at(name).cls.findVarInCurrent(id, depth + 1);
            }
            return nullptr;
        }
        Func* Class::findFuncInCurrent(ID id, uint depth) {
            if (id.length() == depth) {
                string name(id.name());
                if (funcs.contains(name)) {
                    return &funcs.at(name).func;
                }
                return nullptr;
            }
            string name(id.context(depth));
            if (classes.contains(name)) {
                return classes.at(name).cls.findFuncInCurrent(id, depth + 1);
            }
            return nullptr;
        }
        Class* Class::findClassInCurrent(ID id, uint depth) {
            if (id.length() == depth) {
                string name(id.name());
                if (classes.contains(name)) {
                    return &classes.at(name).cls;
                }
                return nullptr;
            }
            string name(id.context(depth));
            if (classes.contains(name)) {
                return classes.at(name).cls.findClassInCurrent(id, depth + 1);
            }
            return nullptr;
        }
    }
}