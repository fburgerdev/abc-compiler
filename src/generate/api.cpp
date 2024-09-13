#include "api.hpp"

namespace Compiler {
    namespace CPP {
        // Type
        void Type::generate(ostream& stream) const {
            if (qualifier == Qualifier::CONST) {
                stream << "const ";
            }
            if (std::holds_alternative<Primitive>(value)) {
                switch (std::get<Primitive>(value)) {
                case Primitive::VOID:
                    stream << "void";
                    break;
                case Primitive::BOOL:
                    stream << "bool";
                    break;
                case Primitive::CHAR:
                    stream << "char";
                    break;
                case Primitive::INT:
                    stream << "int";
                    break;
                case Primitive::UINT:
                    stream << "unsigned int";
                    break;
                case Primitive::FLOAT:
                    stream << "float";
                    break;
                case Primitive::DOUBLE:
                    stream << "double";
                    break;
                default:
                    break;
                }
            }
            else if (std::holds_alternative<const Class*>(value) && std::get<const Class*>(value)) {
                // stream << "CLASS";
                stream << std::get<const Class*>(value)->name; // TODO
            }
            else {
                stream << "UNRESOLVED";
            }
            if (valueCategory == ValueCategory::LVALUE) {
                stream << "&";
            }
            else if (valueCategory == ValueCategory::RVALUE) {
                stream << "&&";
            }
        }

        // Var
        void Var::generate(ostream& stream) const {
            for (const string& name : context.namespaces) {
                stream << "namespace " << name << '{';
            }
            if (specifier == Specifier::STATIC) {
                stream << "static ";
            }
            else if (specifier == Specifier::EXTERN) {
                stream << "extern ";
            }
            else if (specifier == Specifier::STATIC_INLINE) {
                stream << "static inline ";
            }
            type.generate(stream);
            stream << ' ' << name;
            for (uint i = 0; i < context.namespaces.size(); ++i) {
                stream << '}';
            }
        }

        // Func
        void Func::generate(ostream& stream) const {
            for (const string& name : context.namespaces) {
                stream << "namespace " << name << '{';
            }
            if (storage == Storage::STATIC) {
                stream << "static ";
            }
            else if (storage == Storage::EXTERN) {
                stream << "extern ";
            }
            stream << name;
            stream << '(';
            for (uint i = 0; i < params.size(); ++i) {
                params[i].generate(stream);
                if (i + 1 < params.size()) {
                    stream << ',';
                }
            }
            stream << ')';
            if (isConst) {
                stream << "const ";
            }
            if (returnType.has_value()) {
                returnType.value().generate(stream);
            }
            for (uint i = 0; i < context.namespaces.size(); ++i) {
                stream << '}';
            }
        }

        // Class
        void Class::generate(ostream& stream) const {
            for (const string& name : context.namespaces) {
                stream << "namespace " << name << '{';
            }
            stream << "class " << name << '{';
            Access lastAccess = Access::PRIVATE;
            for (auto [access, api] : order) {
                if (access != lastAccess) {
                    if (access == Access::PUBLIC) {
                        stream << "public:";
                    }
                    else if (access == Access::PRIVATE) {
                        stream << "private:";
                    }
                    lastAccess = access;
                }
                api->generate(stream);
                stream << ';';
            }
            stream << '}';
            for (uint i = 0; i < context.namespaces.size(); ++i) {
                stream << '}';
            }
        }

        // Global
        void Global::generate(ostream& stream) const {
            for (const auto& api : order) {
                api->generate(stream);
                stream << ';';
            }
        }
    }
}