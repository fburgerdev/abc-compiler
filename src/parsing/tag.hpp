#pragma once
#include "common.hpp"

namespace Compiler {
    // Tag
    class Tag {
    public:
        // constructor
        Tag(strview str);

        // str
        strview str() const;
        // match
        bool match(strview query) const;
        // compare
        auto operator<=>(const Tag& other) const = default;
    private:
        string _str;
    };
}