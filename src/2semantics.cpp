#include "parsing/node.hpp"
#include "semantics/source.hpp"

namespace Compiler {
    // static
    static Map<Variant<Var*, Func*>, const Node*> s_codeMap;

    // ID
    template<>
    ID::ID<Node>(const Node& node, Handler& handler) {
        _view = node.view();
    }

    // Type
    template<>
    Type::Type<Node>(const Node& node, Handler& handler) {
        // qualifier
        if (node.contains("kw/mut")) {
            qualifier = Qualifier::MUT;
        }
        else if (node.contains("kw/const")) {
            qualifier = Qualifier::CONST;
        }
        // category
        if (node.contains("lvalue")) {
            category = ValueCategory::LVALUE;
        }
        else if (node.contains("rvalue")) {
            category = ValueCategory::RVALUE;
        }
        // value
        const Node& valueNode =
            node.match("id") ? node :
            node.contains("kw/type") ? node.at("kw/type") :
            node.contains("id") ? node.at("id") : node;
        if (valueNode.match("kw/type")) {
            if (valueNode.match("kw/type/bool")) {
                value = Primitive::BOOL;
            }
            else if (valueNode.match("kw/type/char")) {
                value = Primitive::CHAR;
            }
            else if (valueNode.match("kw/type/int")) {
                value = Primitive::INT;
            }
            else if (valueNode.match("kw/type/uint")) {
                value = Primitive::UINT;
            }
            else if (valueNode.match("kw/type/float")) {
                value = Primitive::FLOAT;
            }
        }
        else if (valueNode.match("id")) {
            value = ID(valueNode, handler);
        }
    }

    // Var
    template<>
    Var::Var<Node>(const Node& node, Context* context, Handler& handler)
        : context(context) {
        const Node& head = node.contains("head") ? node.at("head") : node;
        name = head.front().view();
        if (1 < head.size()) {
            type = Type(head.back(), handler);
        }
        if (node.contains("specifier")) {
            if (node.at("specifier").contains("kw/local")) {
                specifier = Specifier::LOCAL;
            }
            else if (node.at("specifier").contains("kw/global")) {
                specifier = Specifier::GLOBAL;
            }
            else if (node.at("specifier").contains("kw/static")) {
                specifier = Specifier::STATIC;
            }
        }
        if (node.match("api/var/init")) {
            s_codeMap.emplace(this, &node.back());
        }
    }
    // Func
    template<>
    Func::Func<Node>(const Node& node, Context* parent, Handler& handler)
        : Context(parent) {
        const Node& head = node.at("head");
        name = head.front().view();
        if (head.contains("param_list")) {
            for (const Node& param : head.at("param_list")) {
                params.emplace_back(param, this, handler);
            }
        }
        if (head.contains("kw/mut")) {
            qualifier = Qualifier::MUT;
            if (3 < head.size()) {
                returnType = Type(head.back(), handler);
            }
        }
        else if (head.contains("kw/const")) {
            qualifier = Qualifier::CONST;
            if (3 < head.size()) {
                returnType = Type(head.back(), handler);
            }
        }
        else if (head.contains("param_list") && 2 < head.size()) {
            returnType = Type(head.back(), handler);
        }
        else if (!head.contains("param_list") && 1 < head.size()) {
            returnType = Type(head.back(), handler);
        }
        if (node.contains("specifier")) {
            if (node.at("specifier").contains("kw/local")) {
                specifier = Specifier::LOCAL;
            }
            else if (node.at("specifier").contains("kw/global")) {
                specifier = Specifier::GLOBAL;
            }
            else if (node.at("specifier").contains("kw/static")) {
                specifier = Specifier::STATIC;
            }
        }
        s_codeMap.emplace(this, &node.back());
    }
    // Class
    template<>
    Class::Class<Node>(const Node& node, Context* parent, Handler& handler)
        : Context(parent) {
        const Node& head = node.at("head");
        name = head.front().view();
        Access access = Access::PUBLIC;
        for (const Node& child : node) {
            if (child.match("access")) {
                if (child.contains("kw/public")) {
                    access = Access::PUBLIC;
                }
                else if (child.contains("kw/private")) {
                    access = Access::PRIVATE;
                }
            }
            else if (child.match("api")) {
                string childName(child.at("head").front().view());
                if (child.match("api/class")) {
                    classes.try_emplace(childName, access, child, this, handler);
                }
                else if (child.match("api/func")) {
                    if (child.contains("specifier")) {
                        if (child.at("specifier").contains("kw/local")) {
                            //TODO: handler
                        }
                        else if (child.at("specifier").contains("kw/global")) {
                            //TODO: handler
                        }
                    }
                    funcs.try_emplace(childName, access, child, this, handler);
                }
                else if (child.match("api/var")) {
                    if (child.contains("specifier")) {
                        if (child.at("specifier").contains("kw/local")) {
                            //TODO: handler
                        }
                        else if (child.at("specifier").contains("kw/global")) {
                            //TODO: handler
                        }
                    }
                    vars.try_emplace(childName, access, child, this, handler);
                }
            }
        }
        if (node.contains("specifier")) {
            //TODO: handler
        }
    }
    // Group
    template<>
    Group::Group<Node>(const Node& node, Context* parent, Handler& handler)
        : Context(parent) {
        if (node.contains("head")) {
            const Node& head = node.at("head");
            name = head.front().view();
        }
        for (const Node& child : node) {
            if (child.match("head")) {
                continue;
            }
            string childName(child.at("head").front().view());
            if (child.match("api/group")) {
                groups.try_emplace(childName, child, this, handler);
            }
            else if (child.match("api/class")) {
                classes.try_emplace(childName, child, this, handler);
            }
            else if (child.match("api/func")) {
                if (child.contains("specifier")) {
                    if (child.at("specifier").contains("kw/static")) {
                        //TODO: handler
                    }
                }
                funcs.try_emplace(childName, child, this, handler);
            }
            else if (child.match("api/var")) {
                if (child.contains("specifier")) {
                    if (child.at("specifier").contains("kw/static")) {
                        //TODO: handler
                    }
                }
                vars.try_emplace(childName, child, this, handler);
            }
        }
        if (node.contains("specifier")) {
            //TODO: handler
        }
    }

    // Expr
    template<>
    Expr::Expr<Node>(const Node& node, Handler& handler);
    template<>
    Literal::Literal<Node>(const Node& node, Handler& handler) {
        if (node.match("lit/bool")) {
            litType = LiteralType::BOOL;
        }
        else if (node.match("lit/int")) {
            litType = LiteralType::INT;
        }
        else if (node.match("lit/float")) {
            litType = LiteralType::FLOAT;
        }
        else if (node.match("lit/char")) {
            litType = LiteralType::CHAR;
        }
        // else if (node.match("lit/string")) {
        //     litType = LiteralType::STRING;
        // }
        value = node.view();
    }
    template<>
    VarAccess::VarAccess<Node>(const Node& node, Handler& handler) {
        id = ID(node, handler);
    }
    template<>
    MemberAccess::MemberAccess(const Node& node, Handler& handler) {
        // expr = makeUniquePtr<Expr>(node.front(), handler);
        // member = node.back().view();
    }
    template<>
    StaticCall::StaticCall(const Node& node, Handler& handler) {

    }
    template<>
    MemberCall::MemberCall(const Node& node, Handler& handler) {

    }
    template<>
    Expr::Expr<Node>(const Node& node, Handler& handler) {
        if (node.match("lit")) {
            value = Literal(node, handler);
        }
        else if (node.match("self_member") || node.match("expr/member")) {
            value = MemberAccess(node, handler);
        }
        else if (node.match("id")) {
            value = VarAccess(node, handler);
        }
        else if (node.match("expr/member_call")) {
            value = MemberCall(node, handler);
        }
        else {
            value = StaticCall(node, handler);
        }
    }

    // Stmt
    template<>
    LocalDecl::LocalDecl(const Node& node, Handler& handler) {

    }
    template<>
    Cond::Cond(const Node& node, Handler& handler) {
        
    }
    template<>
    ForLoop::ForLoop(const Node& node, Handler& handler) {
        
    }
    template<>
    WhileLoop::WhileLoop(const Node& node, Handler& handler) {
        
    }
    template<>
    Return::Return(const Node& node, Handler& handler) {
        
    }
    template<>
    Stmt::Stmt<Node>(const Node& node, Handler& handler) {
        if (node.match("stmt/var")) {
            value = LocalDecl(node, handler);
        }
        else if (node.match("stmt/cond")) {
            value = Cond(node, handler);
        }
        else if (node.match("stmt/expr")) {
            value = Expr(node, handler);
        }
        else if (node.match("stmt/for")) {
            value = ForLoop(node, handler);
        }
        else if (node.match("stmt/while")) {
            value = WhileLoop(node, handler);
        }
        else if (node.match("stmt/skip")) {
            value = Skip();
        }
        else if (node.match("stmt/break")) {
            value = Break();
        }
        else if (node.match("stmt/return")) {
            value = Return(node, handler);
        }
    }
    template<>
    CodeBlock::CodeBlock<Node>(const Node& node, Context* parent, Handler& handler)
        : Context(parent) {
        for (const Node& child : node) {
            stmts.emplace_back(child, handler);
        }
    }

    // Source
    template<>
    Source::Source<Node>(const Node& node, Handler& handler)
        : global(node, nullptr, handler) {
        for (auto [context, node] : s_codeMap) {
            if (std::holds_alternative<Var*>(context)) {
                varInits.try_emplace(std::get<Var*>(context), *(node), handler);
            }
            else {
                funcBlocks.try_emplace(std::get<Func*>(context), *(node), std::get<Func*>(context), handler);
            }
        }
    }
}