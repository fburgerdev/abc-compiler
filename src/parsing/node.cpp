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
    // size
    uint Node::size() const {
        return std::distance(_begin, _end);
    }
    // at
    Node& Node::at(uint index) {
        return *(_begin + index);
    }
    const Node& Node::at(uint index) const {
        return *(_begin + index);
    }

    // contains
    bool Node::contains(strview query) const {
        for (auto it = _begin; it != _end; ++it) {
            if (it->match(query)) {
                return true;
            }
        }
        return false;
    }
    // at
    Node& Node::at(strview query) {
        for (auto it = _begin; it != _end; ++it) {
            if (it->match(query)) {
                return *it;
            }
        }
        throw std::runtime_error("node not found");
    }
    const Node& Node::at(strview query) const {
        for (auto it = _begin; it != _end; ++it) {
            if (it->match(query)) {
                return *it;
            }
        }
        throw std::runtime_error("node not found");
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
    // setRange
    void Node::setRange(NodeIterator begin, NodeIterator end) {
        _begin = begin;
        _end = end;
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