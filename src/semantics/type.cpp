#include "type.hpp"

namespace Compiler {
    // isBound
    bool Type::isBound() const {
        if (std::holds_alternative<Auto>(value)) {
            return false;
        }
        else if (std::holds_alternative<ID>(value)) {
            return false;
        }
        return true;
    }
}