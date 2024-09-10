#pragma once
#include "common.hpp"

namespace Compiler {
    // prepareTokenize
    Pair<string, Map<uint, string>> prepareTokenize(const List<Pair<string, string>>& rules);
    // regexMatch
    bool regexMatch(strview src, strview pattern, function<void(strview, uint)> func);
}