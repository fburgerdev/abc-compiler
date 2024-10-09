#pragma once
#include <iostream>
#include <fstream>

// primitive types
using v0 = void;
using b8 = bool;
using c8 = char;
using i32 = std::int32_t;
using i64 = std::int64_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using f32 = float;
using f64 = double;

// value categories
template<class T>
class lval {
public:
    // member access
    T* operator->() {
        return _value;
    }
private:
    T* _value;
};
template<class T>
class rval {
public:
    // member access
    T* operator->() {
        return _value;
    }
private:
    T* _value;
};