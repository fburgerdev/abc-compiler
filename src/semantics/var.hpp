#pragma once
#include "type.hpp"
#include "context.hpp"

namespace Compiler {
    // Specifier
    enum class Specifier {
        NONE, LOCAL, GLOBAL, STATIC
    };

    // Var
    class Var {
    public:
        // constructor
        Var() = default;
        template<class Node>
        Var(const Node& node, Context* context, Handler& handler);

        // name, type, qualifier, specifier
        strview name;
        Variant<Auto, Type> type = Auto();
        Qualifier qualifier = Qualifier::NONE;
        Specifier specifier = Specifier::NONE;
        // context
        Context* context = nullptr;
    };
}