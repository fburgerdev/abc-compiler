#include "lexing/token.hpp"
#include "parsing/node.hpp"
#include "parsing/layer.hpp"

namespace Compiler {
    // Node
    template<>
    Node::Node<Token>(const Token& token)
        : tag(Tag(token.name)), _view(token.view) {}
    
    // Layer
    template<>
    Layer::Layer<List<Token>>(const List<Token>& tokens) {
        _nodes.insert(_nodes.end(), tokens.begin(), tokens.end());
    }
}
