#pragma once
#include "handler/handler.hpp"
#include "file/file.hpp"
#include "lexing/abc.hpp"
#include "parsing/abc.hpp"
#include "semantics/source.hpp"
#include "generate/generator_c.hpp"

namespace Compiler {
    inline bool compile(const Filepath& input) {
        // handler
        Handler handler;
        // // :: test
        // handler.push(testError());
        // handler.push(testWarning());
        // handler.flush([]([[maybe_unused]] strview code) {
        //     return CodeSpan{ Location{ 0, 0 }, Location{ 0, 0 } };
        // });

        // file
        File file(input);
        // // :: test
        // strview code(file.view().data() + 40, file.view().data() + 60);
        // cout << code << endl;
        // cout << file.searchSpan(code).value().begin.ln << endl;
        // cout << file.searchSpan(code).value().begin.col << endl;
        // cout << file.searchSpan(code).value().end.ln << endl;
        // cout << file.searchSpan(code).value().end.col << endl;

        // lexing
        Lexer lexer = abcLexer();
        List<Token> tokens = lexer.lex(file.view(), handler);
        if (handler.shouldCancel()) {
            handler.flush([&](strview code) {
                return file.searchSpan(code).value();
            });
            return false;
        }
        // // :: test
        // for (const Token& token : tokens) {
        //     cout << token.name << " : " << token.view << endl;
        // }

        // parsing
        Parser parser = abcParser();
        UniquePtr<Layer> layer = parser.parse(move(tokens), handler);
        if (handler.shouldCancel()) {
            handler.flush([&](strview code) {
                return file.searchSpan(code).value();
            });
            return false;
        }
        // :: test
        for (const Node& node : *layer) {
            node.dump();
        }

        // semantics
        ABC::Source sourceABC(layer->front(), handler);
        sourceABC.bind(handler);
        if (handler.shouldCancel()) {
            handler.flush([&](strview code) {
                return file.searchSpan(code).value();
            });
            return false;
        }
        
        // generate
        IR::Source sourceIR;
        GeneratorC generator(cout);
        generator.generate(sourceIR);
        
        return true;
    }
}