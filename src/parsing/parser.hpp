#pragma once
#include "handler/handler.hpp"
#include "layer.hpp"

namespace Compiler {
    // Parser
    class Parser {
    public:
        // constructor
        Parser(const Set<Tag>& tokens);

        // add
        void add(const Rule& rule);
        // :: recursive
        void addRecursive(const Rule& rule);
        void addRecursive(const List<Rule>& rules);

        // cleanup
        void cleanup(const Set<string>& queries);

        // parse
        UniquePtr<Layer> parse(Layer&& tokenLayer, Handler& handler) const;
    private:
        void updateDepthMap(const Rule& rule);

        List<Pair<Rule, uint>> _rules;
        Set<string> _cleanup;
        Map<Tag, uint> _depthMap;
    };
}