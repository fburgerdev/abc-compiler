#include "uuid.hpp"
#include <random>

namespace Compiler {
    // constructor
    UUID::UUID() {
        std::random_device random_device;
        std::mt19937_64 generator(random_device());
        std::uniform_int_distribution<uint64_t> distribution(0, UINT64_MAX);
        _id = distribution(generator);
    }
    
    // getID
    uint64 UUID::getID() const {
        return _id;
    }
    // <<
    ostream& operator<<(ostream& stream, const UUID& uuid) {
        return stream << '_' << uuid.getID();
    }
}