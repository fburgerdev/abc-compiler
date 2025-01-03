#include "layer.hpp"

namespace Compiler {
    // constructor
    Layer::Layer(UniquePtr<Layer>&& prev, const Rule& rule, uint cacheLevel)
        : _child(move(prev)) {
        createNodes(rule, cacheLevel);
    }

    void Layer::createNodes(const Rule& rule, uint cacheLevel, bool builtCache) {
        _nodes.clear();
        for (auto it = _child->begin(); it != _child->end();) {
            auto match = rule.func(it, _child->end());
            if (match) {
                auto [end, map] = match.value();
                if (map.size() && cacheLevel == 0 && !builtCache) {
                    _child = makeUniquePtr<Layer>(move(_child), rule, 1);
                    return createNodes(rule, 0, true);
                }
                _shouldDiscard = false;
                if (cacheLevel == 0) {
                    emplace(rule.tag, it, end);
                }
                else {
                    for (auto cacheIt = it; cacheIt != end;) {
                        bool inCache = false;
                        for (const auto& [tag, begin, end] : map) {
                            if (begin == cacheIt) {
                                emplace(tag, begin, end);
                                inCache = true;
                                cacheIt = end;
                                break;
                            }
                        }
                        if (!inCache) {
                            emplace(*cacheIt);
                            cacheIt += 1;
                        }
                    }
                }
                it = end;
            }
            else {
                emplace(*it);
                ++it;
            }
        }
    }

    // emplace
    void Layer::emplace(const Node& node) {
        _nodes.emplace_back(node);
    }
    void Layer::emplace(const Tag& tag, NodeIterator begin, NodeIterator end) {
        _nodes.emplace_back(tag, begin, end);
    }

    // size
    uint Layer::size() const {
        return _nodes.size();
    }
    // depth
    uint Layer::depth() const {
        return _child ? _child->depth() + 1: 0;
    }

    // discard
    bool Layer::shouldDiscard() const {
        return _shouldDiscard;
    }
    UniquePtr<Layer> Layer::discard() {
        UniquePtr<Layer> child = move(_child);
        return child;
    }

    // remove
    void Layer::remove(const Set<string>& queries) {
        List<Node> newNodes;
        newNodes.reserve(_nodes.size());
        for (auto it = _nodes.begin(); it != _nodes.end(); ++it) {
            bool matched = false;
            for (auto query : queries) {
                if (it->match(query)) {
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                newNodes.emplace_back(*it);
            }
        }
        _nodes = move(newNodes);

        if (_child) {
            _child->remove(queries);

            auto childIt = _child->begin();
            for (auto it = _nodes.begin(); it != _nodes.end(); ++it) {
                while (childIt->view().begin() < it->view().begin()) {
                    childIt = std::next(childIt);
                }
                auto begin = childIt;
                while (childIt != _child->end() && childIt->view().begin() < it->view().end()) {
                    childIt = std::next(childIt);
                }
                auto end = childIt;
                it->setRange(begin, end);
                if (it->view() == it->front().view() && it->tag.str() == it->front().tag.str()) {
                    *it = it->front();
                }
            }
        }
    }

    // front / back
    Node& Layer::front() {
        return _nodes.front();
    }
    const Node& Layer::front() const {
        return _nodes.front();
    }
    Node& Layer::back() {
        return _nodes.back();
    }
    const Node& Layer::back() const {
        return _nodes.back();
    }
    // begin / end
    NodeIterator Layer::begin() {
        return _nodes.begin();
    }
    CNodeIterator Layer::begin() const {
        return _nodes.begin();
    }
    NodeIterator Layer::end() {
        return _nodes.end();
    }
    CNodeIterator Layer::end() const {
        return _nodes.end();
    }
}