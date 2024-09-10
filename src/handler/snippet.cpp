#include "snippet.hpp"

namespace Compiler {
    // split
    static List<strview> split(strview str, strview delim) {
        List<strview> splitted;
        auto begin = str.begin();
        for (auto it = str.begin(); it != str.end(); ++it) {
            if (strview(it, str.end()).starts_with(delim)) {
                splitted.emplace_back(begin, it);
                begin = it + delim.length();
            }
        }
        if (begin != str.end()) {
            splitted.emplace_back(begin, str.end());
        }
        return splitted;
    }
    // lineBegin
    static string lineBegin(uint line) {
        string number = std::to_string(line);
        string padding = string();
        for (uint i = 0; i < 6 - number.length(); ++i) {
            padding += ' ';
        }
        return padding + number + " | ";
    }
    static string lineBegin() {
        return "       | ";
    }
    // noColor
    static string s_noColor = "\033[0m";

    // log
    void Snippet::log(SpanLookup lookup, strview colorCode) const {
        if (heading.length() > 0) {
            cout << heading << endl;
        };
        Location location = lookup(code).begin;
        for (strview line : split(entireLines(), "\n")) {
            cout << lineBegin(location.ln++);
            string underline = lineBegin();
            underline += colorCode;
            for (auto it = line.begin(); it != line.end(); ++it) {
                if (it < code.begin()) {
                    underline += ' ';
                }
                else if (code.begin() <= it && it < code.end()) {
                    if (code.begin() == it) {
                        cout << colorCode;
                    }
                    underline += '~';
                    if (it + 1 == code.end()) {
                        underline += ' ';
                        underline += note;
                    }
                }
                else if (it == code.end()) {
                    cout << s_noColor;
                }
                cout << *it;
            }
            cout << endl;
            cout << underline << s_noColor << endl;
        }
        cout << endl;
    }
    // entireLines
    strview Snippet::entireLines() const {
        auto begin = code.begin();
        while (*begin != '\n' && *begin != '\x02') {
            --begin;
        }
        auto end = code.end();
        while (*end != '\n' && *end != '\x03') {
            ++end;
        }
        return strview(begin + 1, end);
    }
}