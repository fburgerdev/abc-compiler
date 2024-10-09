#pragma once
#include "handler/handler.hpp"
#include "group.hpp"
#include "stmt.hpp"

namespace Compiler {
    // bindTypes
    void bindTypes(Var& var, Handler& handler);
    void bindTypes(Func& func, Handler& handler);
    void bindTypes(Class& cls, Handler& handler);
    void bindTypes(Group& grp, Handler& handler);

    // bindExprs
    // :: exprs
    void bindExprs(Literal& literal, Context& context, Handler& handler);
    void bindExprs(VarAccess& varAccess, Context& context, Handler& handler);
    void bindExprs(MemberAccess& memberAccess, Context& context, Handler& handler);
    void bindExprs(StaticCall& staticCall, Context& context, Handler& handler);
    void bindExprs(MemberCall& memberCall, Context& context, Handler& handler);
    void bindExprs(Expr& expr, Context& context, Handler& handler);
    // :: stmts
    void bindExprs(LocalDecl& localDecl, Context& context, Handler& handler);
    void bindExprs(Cond& cond, Context& context, Handler& handler);
    void bindExprs(ForLoop& forLoop, Context& context, Handler& handler);
    void bindExprs(WhileLoop& whileLoop,Context& context, Handler& handler);
    void bindExprs(Skip& skip_, Context& context, Handler& handler);
    void bindExprs(Break& break_, Context& context, Handler& handler);
    void bindExprs(Return& return_, Context& context, Handler& handler);
    void bindExprs(Stmt& stmt, Context& context, Handler& handler);
    // :: block
    void bindExprs(CodeBlock& codeBlock, Context& context, Handler& handler);
}