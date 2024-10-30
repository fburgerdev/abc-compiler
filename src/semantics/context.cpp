#include "context.hpp"

namespace Compiler {
    namespace ABC {
        // constructor
        Context::Context(Context* parent)
            : _parent(parent) {}
        // isGlobal
        bool Context::isGlobal() const {
            return _parent == nullptr;
        }
        // find
        Var* Context::findVar(ID id) {
            if (Var* var = findVarInCurrent(id, 0)) {
                return var;
            }
            else if (_parent) {
                return _parent->findVar(id);
            }
            else {
                return nullptr;
            }
        }
        Func* Context::findFunc(ID id) {
            if (Func* func = findFuncInCurrent(id, 0)) {
                return func;
            }
            else if (_parent) {
                return _parent->findFunc(id);
            }
            else {
                return nullptr;
            }
        }
        Class* Context::findClass(ID id) {
            if (Class* cls = findClassInCurrent(id, 0)) {
                return cls;
            }
            else if (_parent) {
                return _parent->findClass(id);
            }
            else {
                return nullptr;
            }
        }
        // findInCurrent
        Var* Context::findVarInCurrent([[maybe_unused]] ID id, [[maybe_unused]] uint depth) {
            return nullptr;
        }
        Func* Context::findFuncInCurrent([[maybe_unused]] ID id, [[maybe_unused]] uint depth) {
            return nullptr;
        }
        Class* Context::findClassInCurrent([[maybe_unused]] ID id, [[maybe_unused]] uint depth) {
            return nullptr;
        }
    }
}