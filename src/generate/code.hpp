#pragma once
#include "header.hpp"

namespace Compiler {
    namespace CPP {
        // forward
        class Expr;

        // Literal
        class Literal {
        public:
            // constructor
            Literal() = default;
            template<class T>
            Literal(const T& lit);
            // <<
            friend ostream& operator<<(ostream& stream, const Literal& lit);

            // value
            string value;
        };
        // VarAccess
        class VarAccess {
        public:
            // constructor
            VarAccess() = default;
            template<class T>
            VarAccess(const T& access);
            // <<
            friend ostream& operator<<(ostream& stream, const VarAccess& expr);

            // var
            Var* var;
        };
        // MemberAccess
        class MemberAccess {
        public:
            // constructor
            MemberAccess() = default;
            template<class T>
            MemberAccess(const T& access);
            // <<
            friend ostream& operator<<(ostream& stream, const MemberAccess& expr);

            // expr / member
            UniquePtr<Expr> expr;
            string member;
        };
        // Call
        class Call {
        public:
            // constructor
            Call() = default;
            template<class T>
            Call(const T& call);
            // <<
            friend ostream& operator<<(ostream& stream, const Call& expr);

            // func / args
            Func* func = nullptr;
            List<Expr> args;
        };

        // Expr
        class Expr {
        public:
            // constructor
            Expr() = default;
            template<class T>
            Expr(const T& expr);
            // <<
            friend ostream& operator<<(ostream& stream, const Expr& expr);

            // value
            Variant<
                Literal,
                VarAccess, MemberAccess,
                Call
            > value;  
        };

        // forward
        class Stmt;

        // CodeBlock
        class CodeBlock {
        public:
            // constructor
            CodeBlock() = default;
            template<class T>
            CodeBlock(const T& block);
            // <<
            friend ostream& operator<<(ostream& stream, const CodeBlock& block);

            // stmts
            List<Stmt> stmts;
        };

        // LocalDecl
        class LocalDecl {
        public:
            // constructor
            LocalDecl() = default;
            template<class T>
            LocalDecl(const T& decl);
            // <<
            friend ostream& operator<<(ostream& stream, const LocalDecl& stmt);

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
            Cond(const T& cond);
            // <<
            friend ostream& operator<<(ostream& stream, const Cond& stmt);

            Pair<Expr, CodeBlock> ifCond;
            List<Pair<Expr, CodeBlock>> elifConds;
            Opt<CodeBlock> elseBlock;
        };
        // ForLoop
        class ForLoop {
        public:
            // constructor
            ForLoop() = default;
            template<class T>
            ForLoop(const T& loop);
            // <<
            friend ostream& operator<<(ostream& stream, const ForLoop& stmt);

            // decl / cond / step
            Opt<LocalDecl> decl;
            Opt<Expr> cond, step;
            CodeBlock block;
        };
        // Continue
        class Continue {
        public:
            // constructor
            Continue() = default;
            template<class T>
            Continue(const T& continue_);
            // <<
            friend ostream& operator<<(ostream& stream, const Continue& stmt);
        };
        // Break
        class Break {
        public:
            // constructor
            Break() = default;
            template<class T>
            Break(const T& break_);
            // <<
            friend ostream& operator<<(ostream& stream, const Break& stmt);
        };
        // Return
        class Return {
        public:
            // constructor
            Return() = default;
            template<class T>
            Return(const T& return_);
            // <<
            friend ostream& operator<<(ostream& stream, const Return& stmt);

            // expr
            Opt<Expr> expr;
        };
        
        // Stmt
        class Stmt {
        public:
            // constructor
            Stmt() = default;
            template<class T>
            Stmt(const T& stmt);
            // <<
            friend ostream& operator<<(ostream& stream, const Stmt& stmt);

            // value
            Variant<
                LocalDecl, Expr,
                Cond, ForLoop, CodeBlock,
                Continue, Break, Return
            > value;
        };
    }
}