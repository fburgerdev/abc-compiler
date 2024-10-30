#include "id.hpp"

namespace Compiler {
    namespace ABC {
        // name
        strview ID::name() const {
            address index = _view.find_last_of("::");
            if (index == string::npos) {
                return _view;
            }
            else {
                return _view.substr(index + 1);
            }
        }
        // context, length
        strview ID::context() const {
            address index = _view.find_last_of("::");
            if (index == string::npos) {
                return strview();
            }
            else {
                return _view.substr(0, index);
            }
        }
        strview ID::context(uint index) const {
            address found = 0;
            for (uint i = 0; i < index; ++i) {
                address next = _view.find("::", found);
                if (next == string::npos) {
                    // error...
                }
                found = next + strview("::").length();
            }
            return _view.substr(found, _view.find("::", found) - found);
        }
        uint ID::length() const {
            uint count = 0;
            for (address found = 0; _view.find("::", found) < string::npos; ++count) {
                found = _view.find("::", found) + strview("::").length();
            }
            return count;
        }
    }
}