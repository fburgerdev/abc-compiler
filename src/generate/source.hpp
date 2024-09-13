#pragma once
#include "api.hpp"
#include "code.hpp"

namespace Compiler {
    namespace CPP {
        // Source
        struct Source {
            // constructor
            Source() = default;
            template<class ABCSource>
            Source(const ABCSource& abcSource);

            // generateAPI
            void generateAPI(ostream& stream) const;
            // generateCode
            void generateCode(ostream& stream) const;

            // global
            Global global;
        };
    }
}