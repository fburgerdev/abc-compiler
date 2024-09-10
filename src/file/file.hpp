#pragma once
#include "handler/location.hpp"

namespace Compiler {
    // File
    class File {
    public:
        // constructor
        File(const Filepath& path);

        // getPath
        const Filepath& getPath() const;
        // isOk
        bool isOK() const;

        // view
        strview view() const;
        // searchSpan
        Opt<CodeSpan> searchSpan(strview code) const;
    private:
        Filepath _path;
        bool _isOK = true;
        string _str;
    };
}