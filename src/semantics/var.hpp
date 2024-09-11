#pragma once
#include "scope.hpp"

namespace Compiler {
    // Auto
    struct Auto {
        // constructor
        Auto() = default;
    };

    // Var
    struct Var {
        // constructor
        Var() = default;

        // getType
        virtual Type getType() const = 0;
        // dependsOn
        bool dependsOn(const Class& cls) const;

        // name / scope
        string name;
        Scope* scope;
    };

    // StaticVar
    struct StaticVar : public Var {
        // constructor
        StaticVar() = default;
        template<class Node>
        StaticVar(const Node& node, Handler& handler);

        // getType
        virtual Type getType() const override;

        // type
        Variant<Auto, Type> type = Auto();
    };
    // MemberVar
    struct MemberVar : public Var {
        // constructor
        MemberVar() = default;
        template<class Node>
        MemberVar(const Node& node, Handler& handler);

        // getType
        virtual Type getType() const override;

        // type
        Type type;
    };
    // FuncVar
    struct FuncVar : public Var {
        // constructor
        FuncVar() = default;
        template<class Node>
        FuncVar(const Node& node, Handler& handler);

        // getType
        virtual Type getType() const override;

        // type
        Type type;
    };
    // LocalVar
    struct LocalVar : public Var {
        // constructor
        LocalVar() = default;
        template<class Node>
        LocalVar(const Node& node, Handler& handler);

        // getType
        virtual Type getType() const override;

        // type
        Variant<Auto, Type> type = Auto();
    };
}