#pragma once
#include "../scope.hpp"

namespace Compiler {
    // forward
    class Stmt;

    // CodeBlock
    class CodeBlock : public Scope {
    public:
        // constructor
        CodeBlock() = default;
        template<class Node>
        CodeBlock(const Node& node, Handler& handler);
        
        // "Scope" (implementation)
        virtual Class* searchClass(const Identifier& id) override;
        virtual Func* searchFunc(const Identifier& id, const List<Type>& argTypes) override;
        virtual Var* searchVar(const Identifier& id) override;

        // stmts
        List<Stmt> stmts;
    };
}