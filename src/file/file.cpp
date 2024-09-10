#include "file.hpp"

namespace Compiler {
    // constructor
    File::File(const Filepath& path)
        : _path(path) {
        ifstream stream(path);
        if (stream.is_open()) {
            _str += '\x02';
            _str += (strstream() << stream.rdbuf()).str();
            _str += '\x03';
        }
        else {
            _isOK = false;
        }
    }

    // getPath
    const Filepath& File::getPath() const {
        return _path;
    }
    // isOk
    bool File::isOK() const {
        return _isOK;
    }

    // view
    strview File::view() const {
        return strview(_str.begin() + 1, _str.end() - 1);
    }
    // searchSpan
    Opt<CodeSpan> File::searchSpan(strview code) const {
        address start = view().find(code);
        if (start < string::npos) {
            uint rows = 1, cols = 1;
            for (address i = 0; i < start; ++i) {
                if (view().at(i) == '\n') {
                    rows += 1;
                    cols = 1;
                }
                else {
                    cols += 1;
                }
            }
            Location begin{ rows, cols };
            for (address i = 0; i < code.size(); ++i) {
                if (view().at(start + i) == '\n') {
                    rows += 1;
                    cols = 1;
                }
                else {
                    cols += 1;
                }
            }
            Location end{ rows, cols };
            return CodeSpan{ begin, end };
        }
        return std::nullopt;
    }
}