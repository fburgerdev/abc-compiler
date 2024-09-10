#pragma once
#include "snippet.hpp"

namespace Compiler {
    // Msg
    struct Msg {
        // log
        void log(SpanLookup lookup, strview colorCode) const;

        // name / description / snippets / context / suggestions
        string name, description;
        List<Snippet> snippets;
        List<Snippet> context = {};
        List<string> suggestions = {};
    };
    // Error
    struct Error : public Msg {
        // log
        void log(SpanLookup lookup) const;
    };
    // Warning
    struct Warning : public Msg {
        // log
        void log(SpanLookup lookup) const;
    };
}