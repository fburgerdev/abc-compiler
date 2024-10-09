#pragma once
#include "id.hpp"

namespace Compiler {
    // forward
    class Var;
    class Func;
    class Class;

    // Context
    class Context {
    public:
        // constructor
        Context() = default;
        Context(Context* parent);
        
        // isGlobal
        bool isGlobal() const;
        // findInCurrent
        virtual Var* findVarInCurrent(ID id, uint depth);
        virtual Func* findFuncInCurrent(ID id, uint depth);
        virtual Class* findClassInCurrent(ID id, uint depth);
        // find
        Var* findVar(ID id);
        Func* findFunc(ID id);
        Class* findClass(ID id);
    // private:
        Context* _parent = nullptr;
    };
}