#include "identifier.hpp"

namespace Compiler {
    // access
    strview Identifier::name() const {
        if (_str.find("::") == string::npos) {
            return _str;
        }
        else {
            return _str.substr(_str.find_last_of("::") + string("::").length());
        }
    }
    // at
    strview Identifier::at(uint index) const {
        address found = 0;
        for (uint i = 0; i < index; ++i) {
            found = _str.find("::", found) + 2;
        }
        address end = _str.find("::", found);
        if (found == string::npos) {
            throw std::runtime_error("identifier index out of range");
        }
        return strview(_str.begin() + found, end - found);
    }
    // length
    uint Identifier::length() const {
        uint count = 0;
        address index = 0;
        while (_str.find("::", index) < string::npos) {
            index = _str.find("::", index) + 2;
            ++count;
        }
        return count;
    }
}