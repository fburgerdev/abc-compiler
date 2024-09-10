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

    cout << "** compilation successfull **" << endl;
    return EXIT_SUCCESS;
}