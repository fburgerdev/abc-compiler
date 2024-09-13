#pragma once
#include "var.hpp"

namespace Compiler {
    // // Operator
    // enum class Operator {
    //     // increment / decrement
    //     INC, DEC,
    //     // comparison
    //     EQ, NEQ,
    //     LT, LEQ, GT, GEQ,
    //     // assignment
    //     ASSIGN,
    //     ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN,
    //     // arithmetic
    //     ADD, SUB, MUL, DIV,
    // };

    // Func
    struct Func : public Scope {
        // constructor
        Func() = default;
                
        // setScope
        void setScope(Scope* scope);
        // "Scope" implementation
        virtual Class* searchClass(const Identifier& id) override;
        virtual Func* searchFunc(const Identifier& id, const List<Type>& argTypes) override;
        virtual Var* searchVar(const Identifier& id) override;

        // isOperator
        bool isOperator() const;
        // matches
        bool matches(const List<Type>& argTypes);
        // dependsOn
        bool dependsOn(const Class& cls) const;

        // name / params / returnType
        strview name;
        List<FuncVar> params;
        Type returnType;
    };

    // InitFunc
    struct InitFunc : public Func {
        // constructor
        InitFunc() = default;
        template<class Node>
        InitFunc(const Node& node, Handler& handler);
    };
    // StaticFunc
    struct StaticFunc : public Func {
        // constructor
        StaticFunc() = default;
        template<class Node>
        StaticFunc(const Node& node, Handler& handler);
    };
    // MemberFunc
    struct MemberFunc : public Func {
        // constructor
        MemberFunc() = default;
        template<class Node>
        MemberFunc(const Node& node, Handler& handler);

        // isConst
        bool isConst = false;
    };
}