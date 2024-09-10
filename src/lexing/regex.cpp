#include "regex.hpp"
#include <regex>

namespace Compiler {
    // regexPattern
    Pair<string, Map<uint, string>> prepareTokenize(const List<Pair<string, string>>& rules) {
        Map<uint, string> tokenMap;
        string pattern;
        uint offset = 1;
        for (const auto& [name, regex] : rules) {
            pattern += '(';
            pattern += regex;
            pattern += ')';
            pattern += '|';
            tokenMap.emplace(offset, name);
            offset += std::regex(regex).mark_count() + 1;
        }
        if (pattern.length() > 0) {
            pattern.pop_back();
        }
        return { move(pattern), move(tokenMap) };
    }
    // regexMatch
    bool regexMatch(strview src, strview pattern, function<void(strview, uint)> func) {
        std::regex re(pattern.data(), pattern.size());
        bool matched = false;
        auto begin = std::cregex_iterator(src.begin(), src.end(), re);
        auto end = std::cregex_iterator();
        for (auto it = begin; it != end; ++it) {
            std::cmatch match = *it;
            for (auto index = 1; index < match.size(); ++index) {
                if (match[index].matched) {
                    func(strview(src.data() + match.position(), match.length()), index);
                    break;
                }
            }
            matched = true;
        }
        return matched;
    }
}