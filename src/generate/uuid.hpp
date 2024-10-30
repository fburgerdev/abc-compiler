#pragma once
#include "common.hpp"

namespace Compiler {
    // UUID
    class UUID {
    public:
        // constructor
        UUID();

        // getID
        uint64 getID() const;
        // <<
        friend ostream& operator<<(ostream& stream, const UUID& uuid);
    private:
        uint64 _id = 0;
    };
}