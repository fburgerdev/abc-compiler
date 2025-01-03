#pragma once
#include "layer.hpp"

namespace Compiler {
    // Pattern
    template<typename T>
    concept Pattern = requires(T pattern, NodeIterator begin, NodeIterator end) {
        { pattern.match(begin, end) } -> std::same_as<Opt<NodeIterator>>;
    };

    // CachePattern
    template<class Sub>
    struct CachePattern {
        // constructor
        CachePattern(Tag tag, Sub&& sub)
            : tag(tag), sub(move(sub)) {}

        // match
        Opt<NodeIterator> match(NodeIterator begin, NodeIterator end) const {
            if (begin != end && begin->tag == tag) {
                return ++begin;
            }
            Opt<NodeIterator> submatch = sub.match(begin, end);
            if (submatch) {
                cache = Pair<NodeIterator, NodeIterator>(begin, submatch.value());
            }
            else {
                cache.reset();
            }
            return submatch;
        }
        // foreachQuery
        void foreachQuery(function<void(strview)> func) const {
            func(tag.str());
            sub.foreachQuery(func);
        }
        // extendCache
        void extendCache(List<Tuple<Tag, NodeIterator, NodeIterator>>& list) const {
            sub.extendCache(list);
            if (cache) {
                list.emplace_back(tag, cache->first, cache->second);
            }
        }
        // clearCache
        void clearCache() const {
            sub.clearCache();
            cache.reset();
        }
        // toString
        string toString() const {
            return sub.toString();
        }

        // query
        Tag tag;
        mutable Opt<Pair<NodeIterator, NodeIterator>> cache;
        Sub sub;
    };

    // QueryPattern
    struct QueryPattern {
        // constructor
        QueryPattern(strview query)
            : query(query) {}
        
        // match
        Opt<NodeIterator> match(NodeIterator begin, NodeIterator end) const {
            if (begin == end) {
                return std::nullopt;
            }
            if (begin->tag.match(query)) {
                return ++begin;
            }
            return std::nullopt;
        }
        // foreachQuery
        void foreachQuery(function<void(strview)> func) const {
            func(query);
        }
        // extendCache
        void extendCache([[maybe_unused]] List<Tuple<Tag, NodeIterator, NodeIterator>>& list) const {
            // pass
        }
        // clearCache
        void clearCache() const {
            // pass
        }
        // toString
        string toString() const {
            return query;
        }

        // query
        string query;
    };
    // SeqPattern
    template<uint MIN, uint MAX, class Sub>
    struct SeqPattern {
        // constructor
        SeqPattern(Sub&& sub)
            : sub(move(sub)) {}

        // match
        Opt<NodeIterator> match(NodeIterator begin, NodeIterator end) const {
            NodeIterator it = begin;
            uint count = 0;
            while (true) {
                if (count == MAX) {
                    return it;
                }
                Opt<NodeIterator> submatch = sub.match(it, end);
                if (submatch) {
                    it = submatch.value();
                }
                else {
                    if (MIN <= count) {
                        return it;
                    }
                    return std::nullopt;
                }
                ++count;
            }
        }
        // foreachQuery
        void foreachQuery(function<void(strview)> func) const {
            sub.foreachQuery(func);
        }
        // extendCache
        void extendCache(List<Tuple<Tag, NodeIterator, NodeIterator>>& list) const {
            sub.extendCache(list);
        }
        // clearCache
        void clearCache() const {
            sub.clearCache();
        }
        // toString
        string toString() const {
            if (MIN == 0 && MAX == 0xffffffff) {
                return "(" + sub.toString() + ")*";
            }
            else if (MIN == 1 && MAX == 0xffffffff) {
                return "(" + sub.toString() + ")+";
            }
            return "(" + sub.toString() + "){" + std::to_string(MIN) + "," + std::to_string(MAX) + "}";
        }

        // sub
        Sub sub;
    };
    // Seq0Pattern
    template<class Sub>
    using Seq0Pattern = SeqPattern<0, 0xffffffff, Sub>;
    // Seq1Pattern
    template<class Sub>
    using Seq1Pattern = SeqPattern<1, 0xffffffff, Sub>;
    // AndPattern
    template<class... Subs>
    struct AndPattern {
        // constructor
        AndPattern(Subs&&... subs)
            : subs({ move(subs)... }) {}
        
        // match
        Opt<NodeIterator> match(NodeIterator begin, NodeIterator end) const {
            Opt<NodeIterator> it = begin;
            std::apply([&](const auto&... args) {
                ((it = it.has_value() ? args.match(it.value(), end) : it), ...);
            }, subs);
            return it;
        }
        // foreachQuery
        void foreachQuery(function<void(strview)> func) const {
            std::apply([&](const auto&... args) {
                ((args.foreachQuery(func)), ...);
            }, subs);
        }
        // extendCache
        void extendCache(List<Tuple<Tag, NodeIterator, NodeIterator>>& list) const {
            std::apply([&](const auto&... args) {
                ((args.extendCache(list)), ...);
            }, subs);
        }
        // clearCache
        void clearCache() const {
            std::apply([&](const auto&... args) {
                ((args.clearCache()), ...);
            }, subs);
        }
        // toString
        string toString() const {
            return std::apply([](const auto&... args) {
                string result = ((args.toString() + " ") + ...);
                result.pop_back();
                return result;
            }, subs);
        }

        // subs
        Tuple<Subs...> subs;
    };
    // OrPattern
    template<class... Subs>
    struct OrPattern {
        // constructor
        OrPattern(Subs&&... subs)
            : subs({ move(subs)... }) {}

        // match
        Opt<NodeIterator> match(NodeIterator begin, NodeIterator end) const {
            Opt<NodeIterator> it = std::nullopt;
            std::apply([&](const auto&... args) {
                ((it = it.has_value() ? it : args.match(begin, end)), ...);
            }, subs);
            return it;
        }
        // foreachQuery
        void foreachQuery(function<void(strview)> func) const {
            std::apply([&](const auto&... args) {
                ((args.foreachQuery(func)), ...);
            }, subs);
        }
        // extendCache
        void extendCache(List<Tuple<Tag, NodeIterator, NodeIterator>>& list) const {
            std::apply([&](const auto&... args) {
                ((args.extendCache(list)), ...);
            }, subs);
        }
        // clearCache
        void clearCache() const {
            std::apply([&](const auto&... args) {
                ((args.clearCache()), ...);
            }, subs);
        }
        // toString
        string toString() const {
            return std::apply([](const auto&... args) {
                string result = ((args.toString() + "|") + ...);
                result.pop_back();
                return "(" + result + ")";
            }, subs);
        }

        // subs
        Tuple<Subs...> subs;
    };
    // OptPattern
    template<class Sub>
    struct OptPattern {
        // constructor
        OptPattern(Sub&& sub)
            : sub(move(sub)) {}

        // match
        Opt<NodeIterator> match(NodeIterator begin, NodeIterator end) const {
            Opt<NodeIterator> submatch = sub.match(begin, end);
            if (submatch) {
                return submatch.value();
            }
            return begin;
        }
        // foreachQuery
        void foreachQuery(function<void(strview)> func) const {
            sub.foreachQuery(func);
        }
        // extendCache
        void extendCache(List<Tuple<Tag, NodeIterator, NodeIterator>>& list) const {
            sub.extendCache(list);
        }
        // clearCache
        void clearCache() const {
            sub.clearCache();
        }
        // toString
        string toString() const {
            return sub.toString() + "?";
        }

        // sub
        Sub sub;
    };

    // toPattern
    template<typename T> requires Pattern<T>
    auto toPattern(T&& sub) {
        return sub;
    }
    template<typename T> requires std::convertible_to<T, strview>
    auto toPattern(T str) {
        return QueryPattern(str);
    }
    // SUB
    template<typename Sub>
    auto SUB(strview tagname, Sub&& sub) {
        return CachePattern(Tag(tagname), toPattern(sub));
    }
    template<typename... Subs>
    auto SUB(strview tagname, Subs&&... subs) {
        return CachePattern(Tag(tagname), AND(subs...));
    }
    // QUERY
    auto QUERY(strview str) {
        return QueryPattern(str);
    }
    // AND
    template<typename... Subs>
    auto AND(Subs&&... subs) {
        return AndPattern(toPattern(subs)...);
    }
    // OR
    template<typename... Subs>
    auto OR(Subs&&... subs) {
        return OrPattern(toPattern(subs)...);
    }
    // SEQ
    template<typename Sub>
    auto SEQ(Sub&& sub) {
        return Seq0Pattern(toPattern(sub));
    }
    template<typename... Subs>
    auto SEQ(Subs&&... subs) {
        return Seq0Pattern(AND(subs...));
    }
    // SEQ1
    template<typename Sub>
    auto SEQ1(Sub&& sub) {
        return Seq1Pattern(toPattern(sub));
    }
    template<typename... Subs>
    auto SEQ1(Subs&&... subs) {
        return Seq1Pattern(AND(subs...));
    }
    // OPT
    template<typename Sub>
    auto OPT(Sub&& sub) {
        return OptPattern(toPattern(sub));
    }
    template<typename... Subs>
    auto OPT(Subs&&... subs) {
        return OptPattern(AND(subs...));
    }
}