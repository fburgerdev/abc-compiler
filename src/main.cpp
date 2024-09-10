#include "abc-compiler.hpp"

using namespace Compiler;
int main() {
    // handler
    Handler handler;
    // // :: test
    // handler.push(testError());
    // handler.push(testWarning());
    // handler.flush([]([[maybe_unused]] strview code) {
    //     return CodeSpan{ Location{ 0, 0 }, Location{ 0, 0 } };
    // });

    cout << "** compilation successfull **" << endl;
    return EXIT_SUCCESS;
}