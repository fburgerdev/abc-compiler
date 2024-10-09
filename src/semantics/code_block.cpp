#include "code_block.hpp"
#include "stmt.hpp"

namespace Compiler {
    // findInCurrent
    Var* CodeBlock::findVarInCurrent(ID id, [[maybe_unused]] uint depth) {
        if (id.length() == 0) {
            for (auto& stmt : stmts) {
                if (std::holds_alternative<LocalDecl>(stmt.value)) {
                    auto& localDecl = std::get<LocalDecl>(stmt.value);
                    if (localDecl.var.name == id.name()) {
                        return &localDecl.var;
                    }
                }
            }
        }
        return nullptr;
    }
}