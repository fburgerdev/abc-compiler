#include "header.hpp"

namespace Compiler {
    namespace CPP {
        // Type
        ostream& operator<<(ostream& stream, const Type& type_) {
            if (type_.category == Category::LVALUE) {
                stream << "lval<";
            }
            else if (type_.category == Category::RVALUE) {
                stream << "rval<";
            }
            if (type_.qualifier == Qualifier::CONST) {
                stream << "const ";
            }
            if (std::holds_alternative<Primitive>(type_.value)) {
                switch (std::get<Primitive>(type_.value)) {
                case Primitive::VOID:
                    stream << "v0";
                    break;
                case Primitive::BOOL:
                    stream << "b8";
                    break;
                case Primitive::CHAR:
                    stream << "c8";
                    break;
                case Primitive::I32:
                    stream << "i32";
                    break;
                case Primitive::I64:
                    stream << "i64";
                    break;
                case Primitive::U32:
                    stream << "u32";
                    break;
                case Primitive::U64:
                    stream << "u64";
                    break;
                case Primitive::F32:
                    stream << "f32";
                    break;
                case Primitive::F64:
                    stream << "f64";
                    break;
                default:
                    break;
                }
            }
            else if (std::holds_alternative<string>(type_.value)) {
                stream << std::get<string>(type_.value);
            }
            if (type_.category != Category::NONE) {
                stream << '>';
            }
            return stream;
        }
        
        // Var
        ostream& operator<<(ostream& stream, const Var& var_) {
            if (var_.storage == Storage::EXTERN) {
                stream << "extern ";
            }
            else if (var_.storage == Storage::STATIC) {
                stream << "static ";
            }
            stream << var_.type << ' ' << var_.name;
            return stream;
        }
        // Func
        ostream& operator<<(ostream& stream, const Func& func_) {
            if (func_.storage == Storage::EXTERN) {
                stream << "extern ";
            }
            else if (func_.storage == Storage::STATIC) {
                stream << "static ";
            }
            stream << func_.returnType << ' ' << func_.name << '(';
            for (uint i = 0; i < func_.params.size(); ++i) {
                stream << func_.params[i];
                if (i + 1 < func_.params.size()) {
                    stream << ',';
                }
            }
            stream << ')';
            return stream;
        }
        // Struct
        ostream& operator<<(ostream& stream, const Struct& struct_) {
            stream << "struct " << struct_.name << '{';
            for (auto& member : struct_.members) {
                stream << member << ';';
            }
            stream << "};";
            return stream;
        }

        // Header
        ostream& operator<<(ostream& stream, const Header& header_) {
            for (auto& elem : header_.structs) {
                stream << elem;
            }
            for (auto& elem : header_.funcs) {
                stream << elem << ';';
            }
            for (auto& elem : header_.vars) {
                stream << elem << ';';
            }
            return stream;
        }
    }
}