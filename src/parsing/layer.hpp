#pragma once
#include "rule.hpp"

namespace Compiler {
    // Layer
    class Layer {
    public:
        // constructor
        template<class TokenRange>
        Layer(const TokenRange& tokens);
        Layer(UniquePtr<Layer>&& prev, const Rule& rule, uint cacheLevel = 0);
        // copy / move
        Layer(const Layer& other) = delete;
        Layer(Layer&& other) = default;
        auto& operator=(const Layer& other) = delete;
        Layer& operator=(Layer&& other) = default;

        // createNodes
        void createNodes(const Rule& rule, uint cacheLevel, bool builtCache = false);

        // createCache
        UniquePtr<Layer> createCache(const Rule& rule);
        // afterCache
        void afterCache(const Rule& rule);

        // emplace
        void emplace(const Node& node);
        void emplace(const Tag& tag, NodeIterator begin, NodeIterator end);
        // size
        uint size() const;
        // depth
        uint depth() const;

        // discard
        bool shouldDiscard() const;
        UniquePtr<Layer> discard();

        // remove
        void remove(const Set<string>& queries);

        // front / back
        Node& front();
        const Node& front() const;
        Node& back();
        const Node& back() const;
        
        // begin / end
        NodeIterator begin();
        CNodeIterator begin() const;
        NodeIterator end();
        CNodeIterator end() const;
    private:
        UniquePtr<Layer> _child = nullptr;
        bool _shouldDiscard = true;
        List<Node> _nodes;
    };
}