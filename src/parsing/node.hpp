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
        // size
        uint size() const;
        // at
        Node& at(uint index);
        const Node& at(uint index) const;

        // contains
        bool contains(strview query) const;
        // at
        Node& at(strview query);
        const Node& at(strview query) const;
                
        // begin / end
        NodeIterator begin();
        CNodeIterator begin() const;
        NodeIterator end();
        CNodeIterator end() const;
        // setRange
        void setRange(NodeIterator begin, NodeIterator end);

        // dump
        void dump(strview indent = "") const;

        // tag
        Tag tag;
    private:
        strview _view;
        NodeIterator _begin, _end;
    };
}