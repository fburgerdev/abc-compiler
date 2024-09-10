#pragma once
#include "msg.hpp"

namespace Compiler {
    // Handler
    class Handler {
    public:
        // push
        void push(const Error& error);
        void push(const Warning& warning);

        // shouldCancel
        bool shouldCancel() const;
        
        // flush
        void flush(SpanLookup lookup);
    private:
        List<Error> _errors;
        List<Warning> _warnings;
    };
}