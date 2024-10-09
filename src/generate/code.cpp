#include "code.hpp"

namespace Compiler {
    namespace CPP {
        // Literal
        ostream& operator<<(ostream& stream, const Literal& lit) {
            stream << lit.value;
            return stream;
        }
        // VarAccess
        ostream& operator<<(ostream& stream, const VarAccess& access) {
            stream << *(access.var);
            return stream;
        }
        // MemberAccess
        ostream& operator<<(ostream& stream, const MemberAccess& access) {
            stream << *(access.expr) << '.' << access.member;
            return stream;
        }
        // Call
        ostream& operator<<(ostream& stream, const Call& call) {
            stream << *(call.func) << '(';
            for (const Expr& arg : call.args) {
                stream << arg;
            }
            stream << ')';
            return stream;
        }
        // Expr
        ostream& operator<<(ostream& stream, const Expr& expr) {
            std::visit([&] (const auto& arg) {
                stream << arg;
            }, expr.value);
            return stream;
        }

        // CodeBlock
        ostream& operator<<(ostream& stream, const CodeBlock& block) {
            for (const Stmt& stmt : block.stmts) {
                stream << stmt;
            }
            return stream;
        }
        // LocalDecl
        ostream& operator<<(ostream& stream, const LocalDecl& stmt) {
            stream << stmt.var << '=' << stmt.expr << ';';
            return stream;
        }
        // Cond
        ostream& operator<<(ostream& stream, const Cond& stmt) {
            stream << "if(" << stmt.ifCond.first << ')' << stmt.ifCond.second;
            for (const auto& [elifHead, elifBody] : stmt.elifConds) {
                stream << "elif(" << elifHead << ')' << elifBody;
            }
            if (stmt.elseBlock) {
                stream << "else" << stmt.elseBlock.value();
            }
            return stream;
        }
        // ForLoop
        ostream& operator<<(ostream& stream, const ForLoop& stmt) {
            stream << "for(";
            if (stmt.decl) {
                stream << stmt.decl.value();
            }
            else {
                stream << ';';
            }
            if (stmt.cond) {
                stream << stmt.cond.value();
            }
            else {
                stream << ';';
            }
            if (stmt.step) {
                stream << stmt.step.value();
            }
            stream << ')' << stmt.block;
            return stream;
        }
        // Continue
        ostream& operator<<(ostream& stream, [[maybe_unused]] const Continue& stmt) {
            stream << "continue;";
            return stream;
        }
        // Break
        ostream& operator<<(ostream& stream, [[maybe_unused]] const Break& stmt) {
            stream << "break;";
            return stream;
        }
        // Return
        ostream& operator<<(ostream& stream, const Return& stmt) {
            stream << "return";
            if (stmt.expr) {
                stream << ' ' << stmt.expr.value();
            }
            stream << ';';
            return stream;
        }
    
        // Stmt
        ostream& operator<<(ostream& stream, const Stmt& stmt) {
            std::visit([&] (const auto& arg) {
                stream << arg;
            }, stmt.value);
            return stream;
        }
    }
}