#pragma once
#include "func.hpp"

namespace Compiler {
    namespace ABC {
        // forward
        class Expr;

        // LiteralType
        enum class LiteralType {
            BOOL, INT, FLOAT, CHAR, //TODO: STRING
        };
        // Literal
        class Literal {
        public:
            // constructor
            Literal() = default;
            template<class T>
            Literal(const T& node, Handler& handler);
            // getType
            Type getType() const;

            // litType / value
            LiteralType litType;
            strview value;
        };

        // VarAccess
        class VarAccess {
        public:
            // constructor
            VarAccess() = default;
            template<class T>
            VarAccess(const T& node, Handler& handler);
            // getType
            Type getType() const;

            // id / boundVar
            ID id;
            Var* boundVar = nullptr;
        };

        // MemberAccess
        class MemberAccess {
        public:
            // constructor
            MemberAccess() = default;
            template<class T>
            MemberAccess(const T& node, Handler& handler);
            // getType
            Type getType() const;

            // member / expr / boundVar
            strview member;
            UniquePtr<Expr> expr = nullptr;
            Var* boundVar = nullptr;
        };

        // StaticCall
        class StaticCall {
        public:
            // constructor
            StaticCall() = default;
            template<class T>
            StaticCall(const T& node, Handler& handler);
            // getType
            Type getType() const;

            // id / args / boundFunc
            ID id;
            List<Expr> args;
            Func* boundFunc = nullptr;
        };

        // MemberCall
        class MemberCall {
        public:
            // constructor
            MemberCall() = default;
            template<class T>
            MemberCall(const T& node, Handler& handler);
            // getType
            Type getType() const;

            // member / expr / args / boundFunc
            strview member;
            UniquePtr<Expr> expr = nullptr;
            List<Expr> args;
            Func* boundFunc = nullptr;
        };

        // Expr
        class Expr {
        public:
            // constructor
            Expr() = default;
            template<class T>
            Expr(const T& node, Handler& handler);
            // getType
            Type getType() const;

            // value
            Variant<
                Literal,
                VarAccess, MemberAccess,
                StaticCall, MemberCall
            > value;
        };
    }
}