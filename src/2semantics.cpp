#include "parsing/node.hpp"
#include "semantics/code/source.hpp"

namespace Compiler {
    // Identifier
    template<>
    Identifier::Identifier<Node>(const Node& node, Handler& handler) {
        _str = node.view();
    }

    // Type
    template<>
    Type::Type<Node>(const Node& node, Handler& handler) {
        const Node* valueNode = &node;
        if (node.match("type")) {
            if (node.contains("kw/mut")) {
                typeQualifier = TypeQualifier::MUTABLE;
            }
            else if (node.contains("kw/const")) {
                typeQualifier = TypeQualifier::CONSTANT;
            }

            if (node.contains("kw/type")) {
                valueNode = &node.at("kw/type");
            }
            else if (node.contains("id")) {
                valueNode = &node.at("id");
            }

            if (node.contains("lvalue")) {
                valueNode = &node.at("lvalue");
            }
            else if (node.contains("rvalue")) {
                valueNode = &node.at("rvalue");
            }
        }

        if (valueNode->match("kw/type")) {
            if (valueNode->match("kw/type/void")) {
                value = Primitive::VOID;
            }
            else if (valueNode->match("kw/type/bool")) {
                value = Primitive::BOOL;
            }
            else if (valueNode->match("kw/type/char")) {
                value = Primitive::CHAR;
            }
            else if (valueNode->match("kw/type/int")) {
                value = Primitive::INT;
            }
            else if (valueNode->match("kw/type/uint")) {
                value = Primitive::UINT;
            }
            else if (valueNode->match("kw/type/float")) {
                value = Primitive::FLOAT;
            }
        }
        else if (valueNode->match("id")) {
            value = Identifier(*valueNode, handler);
        }
    }

    // StaticVar
    template<>
    StaticVar::StaticVar<Node>(const Node& node, Handler& handler) {
        name = node.front().view();
        if (node.size() == 2) {
            type = Type(node.back(), handler);
        }
    }
    // MemberVar
    template<>
    MemberVar::MemberVar<Node>(const Node& node, Handler& handler) {
        name = node.front().view();
        type = Type(node.back(), handler);
    }
    // FuncVar
    template<>
    FuncVar::FuncVar<Node>(const Node& node, Handler& handler) {
        name = node.front().view();
        type = Type(node.back(), handler);
    }
    // LocalVar
    template<>
    LocalVar::LocalVar<Node>(const Node& node, Handler& handler) {
        name = node.front().view();
        if (node.size() == 2) {
            type = Type(node.back(), handler);
        }
    }

    // InitFunc
    template<>
    InitFunc::InitFunc<Node>(const Node& node, Handler& handler) {
        name = node.front().view();
        if (node.contains("param_list")) {
            for (const Node& child : node.at("param_list")) {
                params.emplace_back(child, handler);
            }
        }
    }
    // StaticFunc
    template<>
    StaticFunc::StaticFunc<Node>(const Node& node, Handler& handler) {
        name = node.front().view();
        if (node.contains("param_list")) {
            for (const Node& child : node.at("param_list")) {
                params.emplace_back(child, handler);
            }
        }
        returnType = Type(node.back(), handler);
    }
    // MemberFunc
    template<>
    MemberFunc::MemberFunc<Node>(const Node& node, Handler& handler) {
        name = node.front().view();
        if (node.contains("param_list")) {
            for (const Node& child : node.at("param_list")) {
                params.emplace_back(child, handler);
            }
        }
        if (node.contains("kw/const")) {
            isConst = true;
        }
        returnType = Type(node.back(), handler);
    }

    // Class
    template<>
    Class::Class<Node>(const Node& node, Handler& handler) {
        name = node.at("head/class").front().view();
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
            if (child.match("api")) {
                string childName(child.at("head").front().view());
                if (child.match("api/class")) {
                    classes.emplace_back(access, Class(child, handler));
                }
                else if (child.match("api/func")) {
                    if (child.contains("head/func/init")) {
                        initFuncs.emplace_back(access, InitFunc(child.at("head"), handler));
                    }
                    else {
                        if (child.contains("deco")) {
                            if (child.at("deco").contains("kw/static")) {
                                staticFuncs.emplace_back(access, StaticFunc(child.at("head"), handler));
                            }
                            else {
                                //TODO: handler
                            }
                        }
                        else {
                            memberFuncs.emplace_back(access, MemberFunc(child.at("head"), handler));
                        }
                    }
                }
                else if (child.match("api/var")) {
                    if (child.contains("deco")) {
                        if (child.at("deco").contains("kw/static")) {
                            staticVars.emplace_back(access, StaticVar(child.at("head"), handler));
                        }
                        else {
                            //TODO: handler
                        }
                    }
                    else {
                        memberVars.emplace_back(access, MemberVar(child.at("head"), handler));
                    }
                }
            }
        }
    }
    
    // Group
    template<>
    Group::Group<Node>(const Node& node, Handler& handler)  {
        if (node.match("api/group")) {
            name = node.at("head/group").front().view();
        }
        for (const Node& child : node) {
            if (child.match("api")) {
                string childName(child.at("head").front().view());
                if (child.match("api/group")) {
                    groups.emplace(childName, Group(child, handler));
                }
                else if (child.match("api/class")) {
                    classes.emplace(childName, Class(child, handler));
                }
                else if (child.match("api/func")) {
                    funcs[childName].emplace_back(child.at("head"), handler);
                }
                else if (child.match("api/var")) {
                    vars.emplace(childName, StaticVar(child.at("head"), handler));
                }
            }
        }
    }

    // Source
    template<>
    Source::Source<Node>(const Node& node, Handler& handler)
        : global(node, handler) {
        global.setScope(nullptr);
    }
}