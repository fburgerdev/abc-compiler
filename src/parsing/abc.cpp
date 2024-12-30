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

        // specifier, qualifier, category
        auto SPECIFIER = OR("kw/local", "kw/global", "kw/static");
        auto QUALIFIER = OR("kw/mut", "kw/const");
        auto CATEGORY = OR("lvalue", "rvalue");

        // id
        parser.add({
            { "id/nested" },
            AND("id", SEQ1("op/scope", "id"))
        });
        // type
        auto TYPE = SUB("type",
            AND(OPT(QUALIFIER), OPT(CATEGORY), OR("kw/type", "id"))
        );
        // typed_id
        parser.add({
            { "typed_id"},
            AND("id", "colon", TYPE)
        });

        // specifier
        parser.add({
            { "specifier" },
            AND("at", SPECIFIER)
        });

        // heads
        // :: var
        parser.add({
            { "head/var/auto" },
            AND("kw/var", "id/single", "colon")
        });
        parser.add({
            { "head/var/type" },
            AND("kw/var", "typed_id")
        });
        // :: func
        auto PARAMS = SUB("params", AND("typed_id", SEQ("comma", "typed_id")));
        parser.add({
            { "head/func/init" },
            AND("kw/init", "a_open", OPT(PARAMS), "a_close")
        });
        parser.add({
            { "head/func/deinit" },
            AND("kw/deinit", "a_open", "a_close")
        });
        parser.add({
            { "head/func/normal" },
            AND("kw/func", "id/single", "a_open", OPT(PARAMS), "a_close", OPT(QUALIFIER), OPT("arrow", TYPE))
        });
        parser.add({
            { "head/func/op" },
            AND("kw/op", "op", "a_open", OPT(PARAMS), "a_close", OPT(QUALIFIER), OPT("arrow", TYPE))
        });
        // :: class
        parser.add({
            { "head/class" },
            AND("kw/class", "id/single")
        });
        // :: group
        parser.add({
            { "head/group" },
            AND("kw/group", "id/single")
        });

        // exprs
        auto EXPR = OR(
            "expr",
            SUB("expr/lit", "lit"),
            SUB("expr/static", "id")
        );
        auto ARGS = AND(EXPR, SEQ("comma", EXPR));
        parser.addRecursive({
            // 2
            { // member access
                { "expr/member" },
                AND(EXPR, "op/member", "id/single")
            },
            { // static call
                { "expr/static_call" },
                AND("id", "a_open", OPT(ARGS), "a_close")
            },
            { // member call
                { "expr/member_call" },
                AND("expr/member", "a_open", OPT(ARGS), "a_close")
            },
            { // post-inc / post-dec
                { "expr/unary" },
                AND(EXPR, OR("op/inc", "op/dec"))
            },
            // // 3
            // { // pos
            //     { "expr/pos" },
            //     AND("op/add", EXPR)
            // },
            // { // neg
            //     { "expr/neg" },
            //     AND("op/sub", EXPR)
            // },
            { // not, pre-inc, pre-dec
                { "expr/unary" },
                AND(OR("kw/not", "op/inc", "op/dec"), EXPR)
            },
            // 5
            { // mul, div, mod
                { "expr/binary" },
                AND(EXPR, OR("op/mul", "op/div", "op/mod"), EXPR)
            },
            // 6
            { // add, sub
                { "expr/binary" },
                AND(EXPR, OR("op/add", "op/sub"), EXPR)
            },
            // { // add (disambiguates pos)
            //     { "expr/add" },
            //     AND(EXPR, "pos")
            // },
            // { // sub (disambiguates neg)
            //     { "expr/sub" },
            //     AND(EXPR, "neg")
            // },
            // 9
            { // compare
                { "expr/binary" },
                AND(EXPR, OR("op/gt", "op/gte", "op/lt", "op/lte"), EXPR)
            },
            // 10
            { // eq / neq
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
            { // assign
                { "expr/binary" },
                AND(EXPR, OR("op/assign", "op/add_assign", "op/sub_assign", "op/mul_assign", "op/div_assign", "op/mod_assign"), EXPR)
            },
            // ()
            { // brace
                { "expr/brace" },
                AND("a_open", EXPR, "a_close")
            },
        });

        // stmts
        auto STMT = OR(
            "stmt",
            SUB("stmt/skip", AND("kw/skip", "semicolon")),
            SUB("stmt/break", AND("kw/break", "semicolon")),
            SUB("stmt/expr", AND(EXPR, "semicolon"))
        );
        parser.add({
            { "stmt/local/auto" },
            AND("kw/let", "id/single", "colon", "op/assign", EXPR, "semicolon")
        });
        parser.add({
            { "stmt/local/type" },
            AND("kw/let", "typed_id", "op/assign", EXPR, "semicolon")
        });
        parser.add({
            { "stmt/return" },
            AND("kw/return", OPT(EXPR), "semicolon")
        });
        parser.addRecursive({
            { // block
                { "block" },
                AND("c_open", SEQ(OR(STMT, "block")), "c_close")
            },
            { // cond
                { "stmt/cond" }, 
                AND(SUB("if", AND("kw/if", EXPR, "block")), SEQ(SUB("elif", AND("kw/elif", EXPR, "block"))), OPT(SUB("else", AND("kw/else", "block"))))
            },
            { // for
                { "stmt/for" },
                AND("kw/for", "id", "kw/in", EXPR, "block")
            },
            { // while
                { "stmt/while" },
                AND("kw/while", EXPR, "block")
            },
        });

        // api
        // :: var
        parser.add({
            { "api/var/init" },
            AND(OPT("specifier"), "head/var", "op/assign", EXPR, "semicolon")
        });
        parser.add({
            { "api/var/no_init" },
            AND(OPT("specifier"), "head/var", "semicolon")
        });
        // :: func
        auto INIT_EXPR = SUB("init", AND("id/single", "c_open", ARGS, "c_close"));
        parser.add({
            { "init_list" },
            AND("colon", INIT_EXPR, SEQ("comma", INIT_EXPR))
        });
        parser.add({
            { "api/func" },
            OR(
                AND(OPT("specifier"), "head/func", OPT("init_list"), "block"),
                AND(OPT("specifier"), "head/func", "init_list")
            )
        });
        // :: class
        auto ACCESS = SUB("access", AND(OR("kw/public", "kw/private"), "colon"));
        parser.addRecursive({
            { "api/class" },
            AND("head/class", "c_open", SEQ(OR(ACCESS, "api")), "c_close")
        });
        // :: group
        parser.addRecursive({
            { "api/group" },
            AND("head/group", "c_open", SEQ("api"), "c_close")
        });

        // source
        parser.addRecursive({
            { "source" },
            SEQ1("api")
        });

        // cleanup
        parser.cleanup({
            // api keywords
            "kw/var", "kw/func", "kw/class", "kw/group",
            // special function keywords
            // "kw/init", "kw/deinit",
            "kw/op",
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