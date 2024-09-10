#pragma once
#include "location.hpp"

namespace Compiler {
    // Snippet
    struct Snippet {
        // log
        void log(SpanLookup lookup, strview colorCode) const;
        // entireLines
        strview entireLines() const;

        // heading / code / note
        string heading = {};
        strview code;
        string note = {};
    };
}