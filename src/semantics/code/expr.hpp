#pragma once
#include "../func.hpp"

namespace Compiler {
    // forward
    class Expr;

    // LiteralType
    enum class LiteralType {
        BOOL, INT, FLOAT, CHAR, STRING,
    };
    // Literal
    struct Literal {
        // constructor
        Literal() = default;
        template<class Node>
        Literal(const Node& node, Handler& handler);
        
        // type / value
        LiteralType type;
        strview value;
    };

    // MemberAccess
    struct MemberAccess {
        // constructor
        MemberAccess() = default;
        template<class Node>
        MemberAccess(const Node& node, Handler& handler);

        // name / expr / bound
        strview name;
        UniquePtr<Expr> expr;
        Var* bound = nullptr;
    };
    // NonMemberAccess
    struct NonMemberAccess {
        // constructor
        NonMemberAccess() = default;
        template<class Node>
        NonMemberAccess(const Node& node, Handler& handler);
        
        // identifier / bound
        Identifier identifier;
        Var* bound = nullptr;
    };
    
    // MemberCall
    struct MemberCall {
        // constructor
        MemberCall() = default;
        template<class Node>
        MemberCall(const Node& node, Handler& handler);

        // name / expr / args / bound
        strview name;
        UniquePtr<Expr> expr;
        List<Expr> args;
        Func* bound = nullptr;
    };
    // NonMemberCall
    struct NonMemberCall {
        // constructor
        NonMemberCall() = default;
        template<class Node>
        NonMemberCall(const Node& node, Handler& handler);

        // identifier / subexprs / bound
        Identifier identifier;
        List<Expr> args;
        Func* bound = nullptr;
    };

    // Expr
    struct Expr {
        // constructor
        Expr() = default;
        template<class Node>
        Expr(const Node& node, Handler& handler);

        // value / type
        Variant<
            Literal,
            MemberAccess, NonMemberAccess,
            MemberCall, NonMemberCall
        > value;
        Variant<Auto, Type> type = Auto();
    };
}