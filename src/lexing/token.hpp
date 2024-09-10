#pragma once
#include "common.hpp"

namespace Compiler {
    // Token
    class Token {
    public:
        // name / view
        string name;
        strview view;
    };
}