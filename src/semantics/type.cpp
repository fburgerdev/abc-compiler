#include "type.hpp"

namespace Compiler {
    // isBound
    bool Type::isBound() const {
        return isPrimitive() || isClass();
    }
    // dependsOn
    bool Type::dependsOn(const Class& cls) const {
        if (isClass()) {
            return &(getClass()) == &cls;
        }
        return false;
    }

    // isIdentifier
    bool Type::isIdentifier() const {
        return std::holds_alternative<Identifier>(value);
    }
    // getIdentifier
    const Identifier& Type::getIdentifier() const {
        return std::get<Identifier>(value);
    }

    // isPrimitive
    bool Type::isPrimitive() const {
        return std::holds_alternative<Primitive>(value);
    }
    // getPrimitive
    Primitive Type::getPrimitive() const {
        return std::get<Primitive>(value);
    }

    // isClass
    bool Type::isClass() const {
        return std::holds_alternative<const Class*>(value) && std::get<const Class*>(value);
    }
    // getClass
    const Class& Type::getClass() const {
        return *(std::get<const Class*>(value));
    }
}