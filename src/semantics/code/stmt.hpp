#pragma once
#include "expr.hpp"
#include "code_block.hpp"

namespace Compiler {
    // LocalDecl
    struct LocalDecl {
        // constructor
        LocalDecl() = default;
        template<class Node>
        LocalDecl(const Node& node, Handler& handler);

        // var / assign
        LocalVar var;
        Expr assign;
    };
    // Cond
    struct Cond {
        // constructor
        Cond() = default;
        template<class Node>
        Cond(const Node& node, Handler& handler);

        // ifStmt / elifStmts / elseBlock
        Pair<Expr, CodeBlock> ifStmt;
        List<Pair<Expr, CodeBlock>> elifStmts;
        Opt<CodeBlock> elseBlock;
    };
    // ForLoop
    struct ForLoop {
        // constructor
        ForLoop() = default;
        template<class Node>
        ForLoop(const Node& node, Handler& handler);

        // var / block
        LocalVar var;
        CodeBlock block;
    };
    // WhileLoop
    struct WhileLoop {
        // constructor
        WhileLoop() = default;
        template<class Node>
        WhileLoop(const Node& node, Handler& handler);

        // cond / block
        Expr cond;
        CodeBlock block;
    };

    // Stmt
    struct Stmt {
        // constructor
        Stmt() = default;
        template<class Node>
        Stmt(const Node& node, Handler& handler);
        
        // value
        Variant<
            LocalDecl, Expr,
            Cond, ForLoop, WhileLoop
        > value;
    };
}