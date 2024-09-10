#include "tag.hpp"

namespace Compiler {
    // constructor
    Tag::Tag(strview str)
        : _str(str) {}

    // str
    strview Tag::str() const {
        return _str;
    }
    // match
    bool Tag::match(strview query) const {
        return _str.starts_with(query) && (query.length() == _str.length() || _str.at(query.length()) == '/');
    }
}