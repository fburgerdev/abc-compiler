#pragma once
#include "node.hpp"

namespace Compiler {
    // MatchFunction
    using MatchFunction = function<
        Opt<Pair<NodeIterator, List<Tuple<Tag, NodeIterator, NodeIterator>>>>(NodeIterator,NodeIterator)
    >;

    // Rule
    class Rule {
    public:
        // constructor
        template<typename Pattern>
        Rule(const Tag& tag, const Pattern& pattern)
            : tag(tag), func([=](NodeIterator begin, NodeIterator end) {
            Opt<Pair<NodeIterator, List<Tuple<Tag, NodeIterator, NodeIterator>>>> out;
            pattern.clearCache();
            auto match = pattern.match(begin, end);
            if (match) {
                out = Pair<NodeIterator, List<Tuple<Tag, NodeIterator, NodeIterator>>>(
                    match.value(), {}
                );
                pattern.extendCache(out->second);
            }
            return out;
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