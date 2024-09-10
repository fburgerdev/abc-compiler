#pragma once
#include "token.hpp"
#include "handler/handler.hpp"

namespace Compiler {
    // Lexer
    class Lexer {
    public:
        // addToken
        void addToken(strview name, strview regex);
        // addIgnore
        void addIgnore(strview query);

        // lex
        List<Token> lex(strview src, Handler& handler) const;
    private:
        List<Pair<string, string>> _rules;
        Set<string> _ignore;
    };
}