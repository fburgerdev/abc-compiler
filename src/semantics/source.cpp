#include "source.hpp"
#include "bind.hpp"

namespace Compiler {
    namespace ABC {
        // bind
        void Source::bind(Handler& handler) {
            bindTypes(global, handler);
        }
    }
}