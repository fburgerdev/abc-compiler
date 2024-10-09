#include "source.hpp"
#include "bind.hpp"

namespace Compiler {
    // bind
    void Source::bind(Handler& handler) {
        bindTypes(global, handler);
    }
}