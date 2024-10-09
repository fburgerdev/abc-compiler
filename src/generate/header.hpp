#pragma once
#include "common.hpp"

namespace Compiler {
    namespace CPP {
        // Storage
        enum class Storage {
            NONE, EXTERN, STATIC
        };
        // Qualifier
        enum class Qualifier {
            NONE, CONST
        };
        // Primitive
        enum class Primitive {
            VOID, BOOL, CHAR,
            I32, I64, U32, U64, F32, F64
        };
        // Category
        enum class Category {
            NONE, LVALUE, RVALUE
        };

        // forward
        class Struct;
        // Type
        class Type {
        public:
            // constructor
            Type() = default;
            template<class T>
            Type(const T& type);
            // <<
            friend ostream& operator<<(ostream& stream, const Type& type_);

            // qualifier, value
            Qualifier qualifier = Qualifier::NONE;
            Variant<Primitive, string> value = Primitive::VOID;
            Category category = Category::NONE;
        };
        // Var
        class Var {
        public:
            // constructor
            Var() = default;
            template<class T>
            Var(const T& var);
            // <<
            friend ostream& operator<<(ostream& stream, const Var& var_);

            // storage, name, type
            Storage storage = Storage::NONE;
            string name;
            Type type;
        };
        // Func
        class Func {
        public:
            // constructor
            Func() = default;
            template<class T>
            Func(const T& func);
            // <<
            friend ostream& operator<<(ostream& stream, const Func& func_);

            // storage, name, params, returnType
            Storage storage = Storage::NONE;
            string name;
            List<Var> params;
            Type returnType;
        };
        // Struct
        class Struct {
        public:
            // constructor
            Struct() = default;
            template<class T>
            Struct(const T& cls);
            // <<
            friend ostream& operator<<(ostream& stream, const Struct& struct_);

            // name, members
            string name;
            List<Var> members;
        };

        // Header
        class Header {
        public:
            // constructor
            Header() = default;
            template<class T>
            Header(const T& grp);

            // add
            template<class T>
            void add(const T& val);
            // <<
            friend ostream& operator<<(ostream& stream, const Header& header_);

            // structs, funcs, vars
            List<Struct> structs;
            List<Func> funcs;
            List<Var> vars;
        };
    }
}