#pragma once
#include "class.hpp"

namespace Compiler {
    // Group
    class Group : public Context {
    public:
        // constructor
        Group() = default;
        template<class Node>
        Group(const Node& node, Context* parent, Handler& handler);
        
        // findInCurrent
        virtual Var* findVarInCurrent(ID id, uint depth) override;
        virtual Func* findFuncInCurrent(ID id, uint depth) override;
        virtual Class* findClassInCurrent(ID id, uint depth) override;

        // name
        strview name;
        // groups, classes, funcs, vars
        Map<string, Group> groups;
        Map<string, Class> classes;
        Map<string, Func> funcs;
        Map<string, Var> vars;
    };
}