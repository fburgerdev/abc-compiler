#include "abc-compiler.hpp"

using namespace Compiler;
static Filepath examples = "/home/flo/abc-compiler/example";
int main() {
    // handler
    Handler handler;
    // // :: test
    // handler.push(testError());
    // handler.push(testWarning());
    // handler.flush([]([[maybe_unused]] strview code) {
    //     return CodeSpan{ Location{ 0, 0 }, Location{ 0, 0 } };
    // });

    // file
    File file(examples / "snake.abc");
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
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }
    // // :: test
    // for (const Node& node : *layer) {
    //     node.dump();
    // }

    // semantics
    Source source(layer->front(), handler);
    source.bind(handler);
    if (handler.shouldCancel()) {
        handler.flush([&](strview code) {
            return file.searchSpan(code).value();
        });
        return EXIT_FAILURE;
    }
    
    // generate
    CPP::Source cppSource(source);
    strstream stream;
    cppSource.generateAPI(stream);
    string indent = "";
    bool lastBracket = false;
    for (char ch : stream.str()) {
        if (ch == '}') {
            indent.pop_back();
            cout << '\n' << indent;
        }
        if (ch == '{') {
            cout << ' ';
        }
        if (lastBracket && ch != ';') {
            cout << '\n' << indent;
        }
        lastBracket = false;
        cout << ch;
        if (ch == ')') {
            cout << ' ';
        }
        if (ch == ';') {
            cout << '\n' << indent;
        }
        if (ch == '\n') {
            cout << indent;
        }
        if (ch == '{') {
            indent += '\t';
            cout << '\n' << indent;
        }
        if (ch == '}') {
            lastBracket = true;
        }
    }
    
    cout << "** compilation successfull **" << endl;
    return EXIT_SUCCESS;
}