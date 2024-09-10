#pragma once
#include "node.hpp"

namespace Compiler {
    // MatchFunction
    using MatchFunction = function<
        Opt<NodeIterator>(NodeIterator,NodeIterator)
    >;

    // Rule
    class Rule {
    public:
        // constructor
        template<typename Pattern>
        Rule(const Tag& tag, const Pattern& pattern)
            : tag(tag), func([=](NodeIterator begin, NodeIterator end) {
            return pattern.match(begin, end);
        }) {
            pattern.foreachQuery([&](strview query) {
                queries.emplace(query);
            });
        }

        // tag / func
        Tag tag;
        MatchFunction func;
        Set<string> queries;
    };
}