#pragma once
#include "tag.hpp"

namespace Compiler {
    // forward
    class Node;

    // iterator
    using NodeIterator = List<Node>::iterator;
    using CNodeIterator = List<Node>::const_iterator;

    // Node
    class Node {
    public:
        // constructor
        template<class Token>
        Node(const Token& token);
        Node(const Tag& tag, NodeIterator begin, NodeIterator end);

        // match
        bool match(strview str) const;
        // view
        strview view() const;
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

        // dump
        void dump(strview indent = "") const;

        // tag
        Tag tag;
    // private:
        strview _view;
        NodeIterator _begin, _end;
    };
}