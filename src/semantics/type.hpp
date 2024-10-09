#pragma once
#include "id.hpp"

namespace Compiler {
    // forward
    class Class;

    // Auto
    struct Auto {};
    // Qualifier
    enum class Qualifier {
        NONE, MUT, CONST
    };
    // Primitive
    enum class Primitive {
        BOOL, CHAR, INT, UINT, FLOAT
    };
    // ValueCategory
    enum class ValueCategory {
        NONE, LVALUE, RVALUE
    };

    // Type
    class Type {
    public:
        // constructor
        Type() = default;
        template<class Node>
        Type(const Node& node, Handler& handler);

        // isBound
        bool isBound() const;

        // qualifier, category, value
        Qualifier qualifier = Qualifier::NONE;
        ValueCategory category = ValueCategory::NONE;
        Variant<Auto, ID, Primitive, Class*> value = Auto();
    };
}