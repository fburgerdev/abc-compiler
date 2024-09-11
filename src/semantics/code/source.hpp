#pragma once
#include "handler/handler.hpp"
#include "../group.hpp"
#include "stmt.hpp"

namespace Compiler {
    // Source
    struct Source {
        // constructor
        Source() = default;
        template<class Node>
        Source(const Node& node, Handler& handler);

        // resolveTypes
        bool resolveTypes(Handler& handler);
        // resolveExprs
        bool resolveExprs(Handler& handler);

        // global / varInits / funcBlocks
        Group global;
        Map<Var*, Expr> varInits;
        Map<InitFunc*, Map<MemberVar*, List<Expr>>> initLists;
        Map<Func*, CodeBlock> funcBlocks;
    };
}