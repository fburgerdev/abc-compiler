#pragma once
// ranges
#include <ranges> // stdr
// types
#include <memory> // address, int, uint
#include <string> // string
#include <string_view> // strview
// containers
#include <array> // Array
#include <vector> // List
#include <stack> // Stack
#include <queue> // Queue
#include <deque> // Deque
#include <forward_list> // LinkedList
#include <list> // DLinkedList
#include <unordered_set> // Set
#include <map> // Map
#include <set> // HashSet
#include <unordered_map> // HashMap
// filesystem
#include <filesystem> // Filepath, stdf
// function
#include <functional> // function
// utility
#include <optional> // Opt
#include <variant> // Variant
#include <tuple> // Tuple
// stream
#include <iostream> // cout, cerr, endl
#include <fstream> // fstream, ifstream, ofstream
#include <sstream> // strstream
// thread
#include <mutex> // mutex, recursive_mutex, unique_lock
#include <shared_mutex> // shared_mutex, shared_lock
#include <atomic> // atomic dtypes...

namespace Compiler {
    // ranges
    namespace stdr = std::ranges;

    // types
    // :: address
    using address = std::size_t;
    // :: char
    using schar = signed char;
    using uchar = unsigned char;
    // :: int
    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;
    // :: uint
    using uint = unsigned int;
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;
    // :: float
    using float32 = float;
    using float64 = double;
    // :: string
    using string = std::string;
    using strview = std::string_view;

    // utility
    template<typename T>
    using Opt = std::optional<T>;
    template<typename T1, typename T2>
    using Pair = std::pair<T1, T2>;
    template<typename... TArgs>
    using Tuple = std::tuple<TArgs...>;
    template<typename... TArgs>
    using Variant = std::variant<TArgs...>;
    template<typename T, typename... TArgs>
    inline bool holds(const Variant<TArgs...>& variant) {
        return std::holds_alternative<T>(variant);
    }

    // container
    // :: sequential
    template<typename T, address N>
    using Array = std::array<T, N>;
    template<typename T>
    using List = std::vector<T>;
    template<typename T>
    using Stack = std::stack<T>;
    template<typename T>
    using Queue = std::queue<T>;
    template<typename T>
    using Deque = std::deque<T>;
    // :: linked list
    template<typename T>
    using LinkedList = std::forward_list<T>;
    template<typename T>
    using DLinkedList = std::list<T>;
    // :: tree
    template<typename T>
    using Set = std::set<T>;
    template<typename Key, typename Value>
    using Map = std::map<Key, Value>;
    // :: hash
    template<typename T>
    using HashSet = std::unordered_set<T>;
    template<typename Key, typename Value>
    using HashMap = std::unordered_map<Key, Value>;

    // filesystem
    using Filepath = std::filesystem::path;
    namespace stdf = std::filesystem;

    // move semantics
    using std::forward;
    using std::move;

    // smart pointers
    template<class T, class... Args>
    auto makeUniquePtr(Args&&... args) {
        return std::make_unique<T>(forward<Args>(args)...);
    }
    template<class T, class... Args>
    auto makeSharedPtr(Args&&... args) {
        return std::make_shared<T>(forward<Args>(args)...);
    }
    template<typename T>
    using UniquePtr = std::unique_ptr<T>;
    template<typename T>
    using SharedPtr = std::shared_ptr<T>;
    template<typename T>
    using WeakPtr = std::weak_ptr<T>;

    // function
    using std::function;

    // stream
    // :: iostream
    using std::cout;
    using std::cerr;
    using std::endl;
    using std::istream;
    using std::ostream;
    // :: fstream
    using std::fstream;
    using std::ifstream;
    using std::ofstream;
    // :: stringstream
    using strstream = std::stringstream;
    using istrstream = std::istringstream;
    using ostrstream = std::ostringstream;

    // thread
    using std::mutex;
    using std::recursive_mutex;
    using std::shared_mutex;
    template<typename Mutex>
    using UniqueLock = std::unique_lock<Mutex>;
    template<typename Mutex>
    using SharedLock = std::shared_lock<Mutex>;

    // atomic
    // :: address
    using atomic_address = std::atomic_size_t;
    // :: int
    using atomic_int = std::atomic_int;
    using atomic_int8 = std::atomic_int8_t;
    using atomic_int16 = std::atomic_int16_t;
    using atomic_int32 = std::atomic_int32_t;
    using atomic_int64 = std::atomic_int64_t;
    // :: uint
    using atomic_uint = std::atomic_uint;
    using atomic_uint8 = std::atomic_uint8_t;
    using atomic_uint16 = std::atomic_uint16_t;
    using atomic_uint32 = std::atomic_uint32_t;
    using atomic_uint64 = std::atomic_uint64_t;
}