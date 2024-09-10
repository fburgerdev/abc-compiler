#include "node.hpp"

namespace Compiler {
    // constructor
    Node::Node(const Tag& tag, NodeIterator begin, NodeIterator end)
        : tag(tag), _begin(begin), _end(end) {
        _view = strview(front().view().begin(), back().view().end());
    }

    // match
    bool Node::match(strview str) const {
        return tag.match(str);
    }
    // view
    strview Node::view() const {
        return _view;
    }
    // front / back
    Node& Node::front() {
        return *_begin;
    }
    const Node& Node::front() const {
        return *_begin;
    }
    Node& Node::back() {
        return *(std::prev(_end)); 
    }
    const Node& Node::back() const {
        return *(std::prev(_end)); 
    }
    // begin / end
    NodeIterator Node::begin() {
        return _begin;
    }
    CNodeIterator Node::begin() const {
        return _begin;
    }
    NodeIterator Node::end() {
        return _end;
    }
    CNodeIterator Node::end() const {
        return _end;
    }

    // dump
    void Node::dump(strview indent) const {
        cout << indent << tag.str() << ": ";
        if (_view.find('\n') == string::npos) {
            cout << _view << endl;
        }
        else {
            cout << _view.substr(0, _view.find('\n') - 1) << "..." << endl;
        }
        for (auto it = _begin; it != _end; ++it) {
            it->dump(string(indent) + "  ");
        }
    }
}