#include "format.hpp"

namespace Compiler {
    // formatSource
    void formatSource(strview source, ostream& stream) {
        string indent = "";
        for (uint i = 0; i < source.size(); i++) {
            char ch = source[i];
            if (ch == '\n' && i + 1 == source.size()) {
                continue;
            }
            if (ch == '{') {
                stream << ' ';
            }
            stream << ch;
            if (ch == ',') {
                stream << ' ';
            }
            if (ch == '{') {
                indent += "  ";
            }
            if (i + 1 < source.size() && source[i + 1] == '}') {
                indent.pop_back();
                indent.pop_back();
            }
            if (ch == '{' || ch == ';') {
                stream << '\n' << indent;
            }
            if (ch == '}' && (i + 1 == source.size() || source[i + 1] != ';')) {
                stream << '\n' << indent;
            }
            // if (ch == '{') {
            //     indent.push_back('\t');
            // }
            // if (ch == '}') {
            //     indent.pop_back();
            // }

            // if (ch == '}' || (last == '}' && ch != ';')) {
            //     stream << '\n' << indent;
            // }
            // if (ch == '{') {
            //     stream << ' ';
            // }
            // stream << ch;
            // if (ch == ',') {
            //     stream << ' ';
            // }
            // if (ch == '{' || ch == ';') {
            //     stream << '\n' << indent;
            // }
            // last = ch;
        }
    }
}