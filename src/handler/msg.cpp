#include "msg.hpp"

namespace Compiler {
    // noColor
    static string s_noColor = "\033[0m";

    // log
    void Msg::log(SpanLookup lookup, strview colorCode) const {
        cout << description;
        cout << " (" << colorCode << name << s_noColor << ")" << endl;
        for (const Snippet& snippet : snippets) {
            snippet.log(lookup, colorCode);
        }
        for (const Snippet& snippet : context) {
            snippet.log(lookup, "\033[96m");
        }
        for (const string& suggestion : suggestions) {
            cout << suggestion << endl << endl;
        }
    }

    // Error
    void Error::log(SpanLookup lookup) const {
        static strview color = "\033[91m";
        cout << color << "error" << s_noColor << ": ";
        Msg::log(lookup, color);
    }
    // Warning
    void Warning::log(SpanLookup lookup) const {
        static strview color = "\033[95m";
        cout << color << "warning" << s_noColor << ": ";
        Msg::log(lookup, color);
    }
}