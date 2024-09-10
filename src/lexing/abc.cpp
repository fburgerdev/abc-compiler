#include "abc.hpp"

namespace Compiler {
    // abcLexer
    Lexer abcLexer() {
        Lexer lexer;
        // comments
        lexer.addToken("comment/single", "//[^\n]*");
        lexer.addToken("comment/multi", "/\\*([^*]|\\*+[^*/])*\\*+/");
        // whitespace
        lexer.addToken("whitespace", "\\s+");
        // literals
        lexer.addToken("lit/string", "\"([^\"\\\\]|\\\\.)*\"");
        lexer.addToken("lit/char", "'([^'\\\\]|\\\\.)*'");
        lexer.addToken("lit/float", "[0-9]+\\.[0-9]+");
        lexer.addToken("lit/int", "[0-9]+");
        lexer.addToken("lit/bool", "true|false");
        // keywords
        // :: declarations
        lexer.addToken("kw/var", "\\bvar\\b");
        lexer.addToken("kw/func", "\\bfunc\\b");
        lexer.addToken("kw/class", "\\bclass\\b");
        lexer.addToken("kw/group", "\\bgroup\\b");
        // :: special functions
        lexer.addToken("kw/init", "\\binit\\b");
        lexer.addToken("kw/deinit", "\\bdeinit\\b");
        lexer.addToken("kw/op", "\\bop\\b");
        // :: decorators
        lexer.addToken("kw/local", "\\blocal\\b");
        lexer.addToken("kw/global", "\\bglobal\\b");
        lexer.addToken("kw/static", "\\bstatic\\b");
        // :: access modifiers
        lexer.addToken("kw/public", "\\bpublic\\b");
        lexer.addToken("kw/private", "\\bprivate\\b");
        // :: qualifiers
        lexer.addToken("kw/const", "\\bconst\\b");
        // :: types
        lexer.addToken("kw/void", "\\bvoid\\b");
        lexer.addToken("kw/bool", "\\bbool\\b");
        lexer.addToken("kw/char", "\\bchar\\b");
        lexer.addToken("kw/int", "\\bint\\b");
        lexer.addToken("kw/uint", "\\buint\\b");
        lexer.addToken("kw/float", "\\bfloat\\b");
        // :: statements
        lexer.addToken("kw/let", "\\blet\\b");
        lexer.addToken("kw/if", "\\bif\\b");
        lexer.addToken("kw/elif", "\\belif\\b");
        lexer.addToken("kw/else", "\\belse\\b");
        lexer.addToken("kw/while", "\\bwhile\\b");
        lexer.addToken("kw/for", "\\bfor\\b");
        lexer.addToken("kw/in", "\\bin\\b");
        lexer.addToken("kw/skip", "\\bskip\\b");
        lexer.addToken("kw/break", "\\bbreak\\b");
        lexer.addToken("kw/return", "\\breturn\\b");
        // :: operators
        lexer.addToken("op/and", "\\band\\b");
        lexer.addToken("op/or", "\\bor\\b");
        lexer.addToken("op/not", "\\bnot\\b");

        // operators
        // :: special
        lexer.addToken("op/scope", "::");
        lexer.addToken("op/member", "\\.");
        // increment / decrement
        lexer.addToken("op/inc", "\\+\\+");
        lexer.addToken("op/dec", "--");
        // comparison
        lexer.addToken("op/eq", "==");
        lexer.addToken("op/neq", "!=");
        lexer.addToken("op/lt", "<");
        lexer.addToken("op/lte", "<=");
        lexer.addToken("op/gt", ">");
        lexer.addToken("op/gte", ">=");
        // assignment
        lexer.addToken("op/assign", "=");
        lexer.addToken("op/add_assign", "\\+=");
        lexer.addToken("op/sub_assign", "-=");
        lexer.addToken("op/mul_assign", "\\*=");
        lexer.addToken("op/div_assign", "/=");
        lexer.addToken("op/mod_assign", "%=");
        // arithmetic
        lexer.addToken("op/add", "\\+");
        lexer.addToken("op/sub", "-");
        lexer.addToken("op/mul", "\\*");
        lexer.addToken("op/div", "/");
        lexer.addToken("op/mod", "%");
        
        // value categories
        lexer.addToken("lvalue", "&");
        lexer.addToken("rvalue", "\\&\\&");

        // identifiers / members
        lexer.addToken("identifier", "\\b[a-zA-Z][a-zA-Z0-9_]*\\b");
        lexer.addToken("self_member", "\\b_[a-zA-Z0-9_]*\\b");

        // punctuators
        lexer.addToken("a_open", "\\(");
        lexer.addToken("a_close", "\\)");
        lexer.addToken("comma", ",");
        lexer.addToken("arrow", "->");
        lexer.addToken("colon", ":");
        lexer.addToken("semicolon", ";");
        lexer.addToken("at", "@");
        lexer.addToken("s_open", "\\[");
        lexer.addToken("s_close", "\\]");
        lexer.addToken("c_open", "\\{");
        lexer.addToken("c_close", "\\}");
 
        lexer.addIgnore("comment/single");
        lexer.addIgnore("comment/multi");
        lexer.addIgnore("whitespace");
        return lexer;
    }
}