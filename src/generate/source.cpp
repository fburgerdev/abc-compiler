#include "source.hpp"

namespace Compiler {
    namespace CPP {
        // generateAPI
        void Source::generateAPI(ostream& stream) const {
            cout << "BEGIN API" << endl;
            global.generate(stream);
            cout << endl << "END API" << endl;
        }
        // generateCode
        void Source::generateCode(ostream& stream) const {
            // TODO
        }
    }
}