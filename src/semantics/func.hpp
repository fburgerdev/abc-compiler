#pragma once
#include "var.hpp"

namespace Compiler {
    // Func
    class Func : public Context {
    public:
        // constructor
        Func() = default;
        template<class Node>
        Func(const Node& node, Context* parent, Handler& handler);

        // findInCurrent
        virtual Var* findVarInCurrent(ID id, uint depth) override;

        // name, qualifier, specifier, params
        strview name;
        Qualifier qualifier = Qualifier::NONE;
        Specifier specifier = Specifier::NONE;
        List<Var> params;
        // returnType (nullopt ~ void)
        Opt<Type> returnType;
    };
}