#include "parser.hpp"

namespace Compiler {
    // constructor
    Parser::Parser(const Set<Tag>& tokens) {
        for (auto& token : tokens) {
            _depthMap[token] = 0;
        }
    }

    // add
    void Parser::add(const Rule& rule) {
        _rules.emplace_back(rule, 0);
        updateDepthMap(rule);
    }
    // addRecursive
    void Parser::addRecursive(const Rule& rule) {
        _rules.emplace_back(rule, 1);
        updateDepthMap(rule);
    }
    void Parser::addRecursive(const List<Rule>& rules) {
        for (uint i = 0; i < rules.size(); ++i) {
            _rules.emplace_back(rules[i], rules.size());
            updateDepthMap(rules[i]);
        }
    }

    // cleanup
    void Parser::cleanup(const Set<string>& queries) {
        _cleanup.insert(queries.begin(), queries.end());
    }

    // parse
    UniquePtr<Layer> Parser::parse(Layer&& tokenLayer, Handler& handler) const {
        auto layer = makeUniquePtr<Layer>(move(tokenLayer));
        for (uint i = 0; i < _rules.size();) {
            auto& [rule, recursion] = _rules.at(i);
            if (recursion == 0) {
                layer = makeUniquePtr<Layer>(move(layer), rule);
                if (layer->shouldDiscard()) {
                    layer = layer->discard();
                }
                ++i;
            }
            else {
                bool matched = true;
                while (matched) {
                    matched = false;
                    for (uint depth = 0; depth < recursion; ++depth) {
                        layer = makeUniquePtr<Layer>(move(layer), _rules.at(i + depth).first);
                        if (layer->shouldDiscard()) {
                            layer = layer->discard();
                        }
                        else {
                            matched = true;
                            break;
                        }
                    }
                }
                i += recursion;
            }
        }
        if (layer->size() > 1) {
            Node* errorNode = &layer->front();
            for (auto& node : *layer) {
                if (_depthMap.at(errorNode->tag) > _depthMap.at(node.tag)) {
                    errorNode = &node;
                }
            }

            handler.push(Error({
                .name = "InvalidSyntax",
                .description = "syntax error",
                .snippets = {
                    {
                        .heading = "the following node could not be parsed",
                        .code = errorNode->view(),
                        .note = string(errorNode->tag.str()),
                    }
                },
            }));
        }
        layer->remove(_cleanup);
        return layer;
    }

    void Parser::updateDepthMap(const Rule& rule) {
        _depthMap[rule.tag] = _rules.size() + 1;
        for (const auto& query : rule.queries) {
            for (auto& [entry, depth] : _depthMap) {
                if (entry.match(query)) {
                    depth = _rules.size();
                }
            }
            _depthMap[Tag(query)] = _rules.size();
        }
    }
} 