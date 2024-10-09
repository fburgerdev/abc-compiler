#include "func.hpp"

namespace Compiler {
    // findInCurrent
    Var* Func::findVarInCurrent(ID id, uint depth) {
        if (id.length() == depth) {
            for (auto& param : params) {
                if (param.name == id.name()) {
                    return &param;
                }
            }
        }
        return nullptr;
    }
}