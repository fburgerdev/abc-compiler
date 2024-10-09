#pragma once
#include "context.hpp"

namespace Compiler {
    // forward
    class Stmt;

    // CodeBlock
    class CodeBlock : public Context {
    public:
        // constructor
        CodeBlock() = default;
        template<class T>
        CodeBlock(const T& node, Context* parent, Handler& handler);

        // findInCurrent
        virtual Var* findVarInCurrent(ID id, uint depth) override;

        // stmts
        List<Stmt> stmts;
    };
}