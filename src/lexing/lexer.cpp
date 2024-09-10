#include "lexer.hpp"
#include "regex.hpp"

namespace Compiler {
    // addToken
    void Lexer::addToken(strview name, strview regex) {
        _rules.emplace_back(name, regex);
    }
    // addIgnore
    void Lexer::addIgnore(strview query) {
        _ignore.emplace(query);
    }

    // lex
    List<Token> Lexer::lex(strview src, Handler& handler) const {
        List<Token> tokens;
        List<Snippet> snippets;
        Token lastIgnore(string(), src);
        auto [pattern, tokenMap] = prepareTokenize(_rules);
        regexMatch(src, pattern, [&] (strview match, uint index) {
            string name = tokenMap.at(index);
            auto begin = std::max(
                lastIgnore.view.end(),
                (tokens.size() ? tokens.back().view.end() : src.begin())
            );
            auto end = match.begin();
            if (begin < end) {
                snippets.push_back({
                    .heading = "Unrecognized Token",
                    .code = strview(begin, end),
                    .note = "unrecognized",
                });
            }
            if (_ignore.contains(name)) {
                lastIgnore = Token(name, match);
            }
            else {
                tokens.emplace_back(name, match);
            }
        });
        if (snippets.size() > 0) {
            handler.push(Error({
                .name = "UnknownToken",
                .description = "part of code could not be tokenized",
                .snippets = snippets,
            }));
        }
        return tokens;
    }
}