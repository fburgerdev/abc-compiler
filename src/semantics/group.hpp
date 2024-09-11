#pragma once
#include "class.hpp"

namespace Compiler {
    // Group
    struct Group : public Scope {
        // constructor
        Group() = default;
        template<class Node>
        Group(const Node& node, Handler& handler);

        // setScope
        void setScope(Scope* scope);
        // "Scope" implementation
        virtual Class* searchClass(const Identifier& id) override;
        virtual Func* searchFunc(const Identifier& id, const List<Type>& argTypes) override;
        virtual Var* searchVar(const Identifier& id) override;
        
        // name / content
        strview name;
        Map<string, Group> groups;
        Map<string, Class> classes;
        Map<string, List<StaticFunc>> funcs;
        Map<string, StaticVar> vars;
    };
}