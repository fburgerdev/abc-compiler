#include "handler.hpp"

namespace Compiler {
    // push
    void Handler::push(const Warning& warning) {
        _warnings.emplace_back(warning);
    }
    void Handler::push(const Error& error) {
        _errors.emplace_back(error);
    }

    // shouldCancel
    bool Handler::shouldCancel() const {
        return _errors.size() > 0;
    }

    // flush
    void Handler::flush(SpanLookup lookup) {
        for (const Warning& warning : _warnings) {
            warning.log(lookup);
        }
        _warnings.clear();
        for (const Error& error : _errors) {
            error.log(lookup);
        }
        _errors.clear();
    }
}