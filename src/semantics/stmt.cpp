#include "stmt.hpp"

namespace Compiler {
    // ForLoop
    Var* ForLoop::findVarInCurrent(ID id, uint depth) {
        if (id.length() == 0 && var.name == id.name()) {
            return &var;
        }
        return CodeBlock::findVarInCurrent(id, depth);
    }
}