#pragma once
#include "common.hpp"

namespace Compiler {
    // Location
    struct Location {
        uint ln, col;
    };
    // CodeSpan
    struct CodeSpan {
        Location begin, end;
    };
    // SpanLookup
    using SpanLookup = function<CodeSpan(strview)>;
}