#pragma once
#include "type.hpp"

namespace Compiler {
    // forward
    class Class;
    class Func;
    class Var;

    // Scope
    class Scope {
    public:
        // constructor
        Scope() = default;
        Scope(Scope* parentScope)
            : parentScope(parentScope) {}

        // search
        virtual Class* searchClass(const Identifier& id) = 0;
        virtual Func* searchFunc(const Identifier& id, const List<Type>& argTypes) = 0;
        virtual Var* searchVar(const Identifier& id) = 0;

        // parentScope
        Scope* parentScope = nullptr;
    };
}