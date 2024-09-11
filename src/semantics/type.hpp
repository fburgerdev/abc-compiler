#pragma once
#include "identifier.hpp"

namespace Compiler {
    // forward
    class Class;

    // Primitive
    enum class Primitive {
        VOID, BOOL, CHAR, INT, UINT, FLOAT,
    };
    
    // TypeQualifier
    enum class TypeQualifier {
        MUTABLE, CONSTANT,
    };
    // ValueCategory
    enum class ValueCategory {
        VALUE, LVALUE, RVALUE,
    };
    
    // Type
    struct Type {
        // constructor
        Type() = default;
        template<class Node>
        Type(const Node& node, Handler& handler);
        Type(const Identifier& identifier)
            : value(identifier) {}
        Type(Primitive value)
            : value(value) {}
        Type(const Class& cls)
            : value(&cls) {}
        
        // isBound
        bool isBound() const;
        // dependsOn
        bool dependsOn(const Class& cls) const;
        
        // isIdentifier
        bool isIdentifier() const;
        // getIdentifier
        const Identifier& getIdentifier() const;

        // isPrimitive
        bool isPrimitive() const;
        // getPrimitive
        Primitive getPrimitive() const;

        // isClass
        bool isClass() const;
        // getClass
        const Class& getClass() const;

        // compare
        auto operator<=>(const Type& other) const = default;

        // qualifier / category / value
        TypeQualifier typeQualifier = TypeQualifier::MUTABLE;
        ValueCategory valueCategory = ValueCategory::VALUE;
        Variant<Identifier, Primitive, const Class*> value = nullptr;
    };
}