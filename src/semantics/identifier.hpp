#pragma once
#include "handler/handler.hpp"

namespace Compiler {
    // Identifier
    class Identifier {
    public:
        // constructor
        Identifier() = default;
        template<class Node>
        Identifier(const Node& node, Handler& handler);

        // compare
        auto operator<=>(const Identifier& other) const = default;
        
        // access
        strview name() const;
        strview at(uint index) const;
        uint length() const;
    private:
        strview _str;
    };
}