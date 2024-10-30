#pragma once
#include "handler/handler.hpp"

namespace Compiler {
    namespace ABC {
        // ID
        class ID {
        public:
            // constructor
            ID() = default;
            template<class Node>
            ID(const Node& node, Handler& handler);

            // name
            strview name() const;
            // context, length
            strview context() const;
            strview context(uint index) const;
            uint length() const;
        private:
            strview _view;
        };
    }
}