#pragma once
#include "handler/msg.hpp"

namespace Compiler {
    // testMsg
    inline Msg testMsg() {
        static string source = "\002func main() -> int {\n  return 0;\n}\003";
        return Msg({
            .name = "TestMsg",
            .description = "This is a test message",
            .snippets = {
                Snippet{
                    .heading = "This is a snippet heading",
                    .code = strview(source.begin() + 16, source.begin() + 19),
                    .note = "snippet note",
                },
                Snippet{
                    .heading = "This is a snippet heading",
                    .code = strview(source.begin() + 24, source.begin() + 30),
                    .note = "snippet note",
                },
            },
            .context = {
                Snippet{
                    .heading = "This is a context heading",
                    .code = strview(source.begin() + 6, source.begin() + 10),
                    .note = "context note",
                },
            },
            .suggestions = {
                "This is a suggestion",
                "This is a suggestion",
            },
        });
    }
    // testError
    inline Error testError() {
        return Error(testMsg());
    }
    // testWarning
    inline Warning testWarning() {
        return Warning(testMsg());
    }
}