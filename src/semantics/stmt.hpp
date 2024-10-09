#pragma once
#include "expr.hpp"
#include "code_block.hpp"

namespace Compiler {
    // LocalDecl
    class LocalDecl {
    public:
        // constructor
        LocalDecl() = default;
        template<class T>
        LocalDecl(const T& node, Handler& handler);
        
        // var / expr
        Var var;
        Expr expr;
    };

    // Cond
    class Cond {
    public:
        // constructor
        Cond() = default;
        template<class T>
        Cond(const T& node, Handler& handler);

        // ifCond / elifConds / elseBlock
        Pair<Expr, CodeBlock> ifCond;
        List<Pair<Expr, CodeBlock>> elifConds;
        Opt<CodeBlock> elseBlock;
    };
    
    // ForLoop
    class ForLoop : public CodeBlock {
    public:
        // constructor
        ForLoop() = default;
        template<class T>
        ForLoop(const T& node, Handler& handler);
        // findInCurrent
        virtual Var* findVarInCurrent(ID id, uint depth) override;

        // var / expr
        Var var;
        Expr expr;
    };
    // WhileLoop
    class WhileLoop : public CodeBlock {
    public:
        // constructor
        WhileLoop() = default;
        template<class T>
        WhileLoop(const T& node, Handler& handler);

        // expr
        Expr expr;
    };

    // Skip
    class Skip {};
    // Break
    class Break {};
    // Return
    class Return {
    public:
        // constructor
        Return() = default;
        template<class T>
        Return(const T& node, Handler& handler);

        // expr
        Opt<Expr> expr;
    };

    // Stmt
    class Stmt {
    public:
        // constructor
        Stmt() = default;
        template<class T>
        Stmt(const T& node, Handler& handler);

        // value
        Variant<
            LocalDecl, Expr,
            Cond, ForLoop, WhileLoop, CodeBlock,
            Skip, Break, Return
        > value;
    };
}