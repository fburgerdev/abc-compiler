#include "abc.hpp"
#include "pattern.hpp"

namespace Compiler {
    // tokenTags
    static Set<Tag> tokenTags({
        // literals
        { "lit/string" }, { "lit/char" }, { "lit/float" }, { "lit/int" }, { "lit/bool" },
        // keywords
        { "kw/var" }, { "kw/func" }, { "kw/class" }, { "kw/group" },
        { "kw/init" }, { "kw/deinit" }, { "kw/op" },
        { "kw/local" }, { "kw/global" }, { "kw/static" },
        { "kw/public" }, { "kw/private" },
        { "kw/mut" }, { "kw/const" },
        { "kw/type/void" }, { "kw/type/bool" }, { "kw/type/char" }, { "kw/type/int" }, { "kw/type/uint" }, { "kw/type/float" },
        { "kw/let" }, { "kw/if" }, { "kw/elif" }, { "kw/else" }, { "kw/while" }, { "kw/for" }, { "kw/in" }, { "kw/skip" }, { "kw/break" }, { "kw/return" },
        { "kw/and" }, { "kw/or" }, { "kw/not" },
        // operators
        { "op/scope" }, { "op/member" },
        { "a_open" }, { "a_close" }, { "comma" }, { "arrow" }, { "colon" }, { "semicolon" }, { "at" }, { "s_open" }, { "s_close" }, { "c_open" }, { "c_close" },
        { "op/inc" }, { "op/dec" },
        { "op/eq" }, { "op/neq" }, { "op/lt" }, { "op/lte" }, { "op/gt" }, { "op/gte" },
        { "op/assign" }, { "op/add_assign" }, { "op/sub_assign" }, { "op/mul_assign" }, { "op/div_assign" },
        { "op/add" }, { "op/sub" }, { "op/mul" }, { "op/div" },
        // value categories
        { "lvalue" }, { "rvalue" },
        // identifiers / members
        { "id/single" }, { "self_member" },
    });

    // abcParser
    Parser abcParser() {
        Parser parser(tokenTags);

        // identifier
        parser.add({ // nested identifier
            { "id/nested" },
            AND("id", SEQ1("op/scope", "id"))
        });

        // type
        auto TYPE = OR("kw/type", "id", "type");
        parser.add({ // type
            { "type" },
            OR(
                AND(OR("kw/mut", "kw/const"), OR("kw/type", "id"), OR("lvalue", "rvalue")),
                AND(OR("kw/mut", "kw/const"), OR("kw/type", "id")),
                AND(OR("kw/type", "id"), OR("lvalue", "rvalue"))
            )
        });

        // heads
        // :: let
        parser.add({ // let head (type)
            { "head/let/type" },
            AND("kw/let", "id/single", "colon", TYPE)
        });
        parser.add({ // let head (auto)
            { "head/let/auto" },
            AND("kw/let", "id/single", "colon")
        });
        // :: var
        parser.add({ // variable head (type)
            { "head/var/type" },
            AND("kw/var", "id/single", "colon", TYPE)
        });
        parser.add({ // variable head (auto)
            { "head/var/auto" },
            AND("kw/var", "id/single", "colon")
        });
        // :: func
        parser.add({ // parameter
            { "param" },
            AND("id/single", "colon", TYPE)
        });
        parser.add({ // parameter list
            { "param_list" },
            AND("param", SEQ("comma", "param"))
        });
        parser.add({ // normal function head
            { "head/func/normal" },
            AND("kw/func", "id/single", "a_open", OPT("param_list"), "a_close", OPT("kw/const"), "arrow", TYPE)
        });
        parser.add({ // init function head
            { "head/func/init" },
            AND("kw/init", "a_open", OPT("param_list"), "a_close")
        });
        parser.add({ // operator function head
            { "head/func/op" },
            AND("kw/op", "op", "a_open", OPT("param_list"), "a_close", OPT("kw/const"), "arrow", TYPE)
        });
        // :: class
        parser.add({ // class head
            { "head/class" },
            AND("kw/class", "id/single")
        });
        // :: group
        parser.add({ // group head
            { "head/group" },
            AND("kw/group", "id/single")
        });

        // expressions
        auto EXPR = OR("lit", "id", "self_member", "expr");
        auto EXPR_LIST = AND(EXPR, SEQ("comma", EXPR));
        parser.addRecursive({
            // 2
            { // member access
                { "expr/member" },
                AND(EXPR, "op/member", "id/single")
            },
            { // static function call
                { "expr/static_call" },
                AND("id", "a_open", OPT(EXPR_LIST), "a_close")
            },
            { // member function call
                { "expr/member_call" },
                AND(OR("self_member", "expr/member"), "a_open", OPT(EXPR_LIST), "a_close")
            },
            { // post-increment / post-decrement
                { "expr/unary" },
                AND(EXPR, OR("op/inc", "op/dec"))
            },
            // 3
            { // positive
                { "expr/pos" },
                AND("op/add", EXPR)
            },
            { // negative
                { "expr/neg" },
                AND("op/sub", EXPR)
            },
            { // not / pre-increment / pre-decrement
                { "expr/unary" },
                AND(OR("op/not", "op/inc", "op/dec"), EXPR)
            },
            // 5
            { // multiplication / division / modulo
                { "expr/binary" },
                AND(EXPR, OR("op/mul", "op/div", "op/mod"), EXPR)
            },
            // 6
            { // addition / subtraction
                { "expr/binary" },
                AND(EXPR, OR("op/add", "op/sub"), EXPR)
            },
            { // addition (disambiguates unary positive)
                { "expr/add" },
                AND(EXPR, "expr/pos")
            },
            { // subtraction (disambiguates unary negative)
                { "expr/sub" },
                AND(EXPR, "expr/neg")
            },
            // 9
            { // comparison
                { "expr/binary" },
                AND(EXPR, OR("op/gt", "op/gte", "op/lt", "op/lte"), EXPR)
            },
            // 10
            { // equality / inequality
                { "expr/binary" },
                AND(EXPR, OR("op/eq", "op/neq"), EXPR)
            },
            // 14
            { // and
                { "expr/binary" },
                AND(EXPR, "kw/and", EXPR)
            },
            // 15
            { // or
                { "expr/binary" },
                AND(EXPR, "kw/or", EXPR)
            },
            // 16
            { // assignment
                { "expr/binary" },
                AND(EXPR, OR("op/assign", "op/add_assign", "op/sub_assign", "op/mul_assign", "op/div_assign", "op/mod_assign"), EXPR)
            },
            // ()
            { // parentheses
                { "expr/brace" },
                AND("a_open", EXPR, "a_close")
            },
        });

        // init list
        parser.add({ // initializer
            { "init" },
            AND("id/single", "c_open", AND(EXPR, SEQ("comma", EXPR)), "c_close")
        });
        parser.add({ // initializer list
            { "init_list" },
            AND("colon", "init", SEQ("comma", "init"))
        });

        // decoration
        parser.add({ // decoration
            { "deco" },
            AND("at", OR("kw/local", "kw/global", "kw/static"))
        });

        // access
        parser.add({ // access modifier
            { "access" },
            AND(OR("kw/public", "kw/private"), "colon")
        });

        // variable
        parser.add({ // variable
            { "api/var" },
            OR(
                AND(OPT("deco"), "head/var", "semicolon"),
                AND(OPT("deco"), "head/var", "op/assign", EXPR, "semicolon")
            )
        });

        // statements
        parser.add({ // if-statement head
            { "if_head" },
            AND("kw/if", EXPR)
        });
        parser.add({ // elif-statement head
            { "elif_head" },
            AND("kw/elif", EXPR)
        });
        parser.add({ // else-statement head
            { "else_head" },
            AND("kw/else") });
        parser.add({ // for-loop head
            { "for_head" },
            AND("kw/for", "id/single", "kw/in", EXPR)
        });
        parser.add({ // while-loop head
            { "while_head" },
            AND("kw/while", EXPR)
        });
        parser.add({ // local variable
            { "stmt/var" },
            AND("head/let", "op/assign", EXPR, "semicolon")
        });
        parser.add({ // skip (control-flow)
            { "stmt/skip" },
            AND("kw/skip", "semicolon")
        });
        parser.add({ // break (control-flow)
            { "stmt/break" },
            AND("kw/break", "semicolon")
        });
        parser.add({ // return (control-flow)
            { "stmt/return" },
            AND("kw/return", OPT(EXPR), "semicolon")
        });
        parser.add({ // expression
            { "stmt/expr" },
            AND(EXPR, "semicolon")
        });
        parser.addRecursive({
            { // code-block
                { "block" },
                AND("c_open", SEQ(OR("stmt", "block")), "c_close")
            },
            { // conditional
                { "stmt/cond" }, 
                AND("if_head", "block", SEQ("elif_head", "block"), OPT("else_head", "block"))
            },
            { // for-loop
                { "stmt/for" },
                AND("for_head", "block")
            },
            { // while-loop
                { "stmt/while" },
                AND("while_head", "block")
            },
        });

        // function
        parser.add({ // function (with block)
            { "api/func" },
            AND(OPT("deco"), "head/func", OPT("init_list"), "block")
        });
        parser.add({ // function (without block but initializer list)
            { "api/func" },
            AND(OPT("deco"), "head/func", "init_list")
        });

        // class
        parser.addRecursive({ // class
            { "api/class" },
            AND("head/class", "c_open", SEQ(OR("access", "api")), "c_close")
        });

        // group
        parser.addRecursive({ // group
            { "api/group" },
            AND("head/group", "c_open", SEQ("api"), "c_close")
        });

        // source
        parser.addRecursive({ // source
            { "source" },
            SEQ1("api")
        });

        // cleanup
        parser.cleanup({
            // api keywords
            "kw/var", "kw/func", "kw/class", "kw/group",
            // special function keywords
            "kw/init", "kw/deinit", "kw/op",
            // statement keywords
            "kw/let", "kw/if", "kw/elif", "kw/else", "kw/while", "kw/for", "kw/in", "kw/skip", "kw/break", "kw/return",
            // special operators
            "op/scope", "op/member",
            // punctuators
            "a_open", "a_close", "comma", "arrow", "colon", "semicolon", "at", "s_open", "s_close", "c_open", "c_close"
        });
        return parser;
    }
}