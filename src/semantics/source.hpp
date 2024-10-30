#pragma once
#include "group.hpp"
#include "stmt.hpp"

namespace Compiler {
    namespace ABC {
        // Source
        class Source {
        public:
            // constructor
            template<class T>
            Source(const T& node, Handler& handler);
            // bind
            void bind(Handler& handler);

            // global
            Group global;
            Map<Var*, Expr> varInits;
            Map<Func*, CodeBlock> funcBlocks;
        };
    }
}