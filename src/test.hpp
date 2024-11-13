#pragma once
#include "compile.hpp"

namespace Compiler {
    static inline string check_mark = "\033[32m\u2714\033[0m";
    static inline string cross_mark = "\033[31m\u2718\033[0m";
    bool test_file(const Filepath& path) {
        cout << "compiling " << path.string() << endl;
        if (compile(path)) {
            cout << check_mark << " " << path.string() << endl;
            return true;
        }
        else {
            cout << cross_mark << " " << path.string() << endl;
            return false;
        }
    }

    bool test(const Filepath& path) {
        uint passed = 0;
        uint failed = 0;
        if (stdf::is_regular_file(path)) {
            if (test_file(path)) {
                passed += 1;
            }
            else {
                failed += 1;
            }
        }
        else if (stdf::is_directory(path)) {
            for (const auto& entry : stdf::recursive_directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    if (test_file(entry.path())) {
                        passed += 1;
                    }
                    else {
                        failed += 1;
                    }
                }
            }
        }
        if (failed == 0) {
            cout << "** all " << passed << " tests passed successfully! **" << endl;
        }
        else {
            cout << "** " << failed << " of " << passed + failed << " tests failed! **" << endl;
        }
        return failed == 0;
    }
}