#include "source.hpp"
#include "uuid.hpp"

namespace Compiler {
    namespace IR {
        // compare
        bool ProductType::operator<(const ProductType& other) const {
            return types < other.types;
        }
        bool SumType::operator<(const SumType& other) const {
            return types < other.types;
        }
        bool Type::operator<(const Type& other) const {
            return qualifier < other.qualifier && value < other.value && category < other.category;
        }

        // addProductTypes
        static void addProductTypes(Set<ProductType>& types, const Type& type) {
            if (std::holds_alternative<ProductType>(type.value)) {
                types.insert(std::get<ProductType>(type.value));
                for (const Type& subtype : std::get<ProductType>(type.value).types) {
                    addProductTypes(types, subtype);
                }
            }
            else if (std::holds_alternative<SumType>(type.value)) {
                for (const Type& subtype : std::get<SumType>(type.value).types) {
                    addProductTypes(types, subtype);
                }
            }
        }
        // getProductTypes
        Set<ProductType> Source::getProductTypes() const {
            Set<ProductType> set;
            for (const auto& [head, body] : funcs) {
                // params
                for (const Type& param : head.params) {
                    addProductTypes(set, param);
                }
                // return type
                addProductTypes(set, head.returnType);
                // local decl
                for (const Stmt& stmt : body.stmts) {
                    if (std::holds_alternative<LocalDecl>(stmt.value)) {
                        addProductTypes(set, std::get<LocalDecl>(stmt.value).type);
                    }    
                }
            }
            return set;
        }
        // addSumTypes
        static void addSumTypes(Set<SumType>& types, const Type& type) {
            if (std::holds_alternative<ProductType>(type.value)) {
                for (const Type& subtype : std::get<ProductType>(type.value).types) {
                    addSumTypes(types, subtype);
                }
            }
            else if (std::holds_alternative<SumType>(type.value)) {
                types.insert(std::get<SumType>(type.value));
                for (const Type& subtype : std::get<SumType>(type.value).types) {
                    addSumTypes(types, subtype);
                }
            }
        }
        // getSumTypes
        Set<SumType> Source::getSumTypes() const {
            Set<SumType> set;
            for (const auto& [head, body] : funcs) {
                // params
                for (const Type& param : head.params) {
                    addSumTypes(set, param);
                }
                // return type
                addSumTypes(set, head.returnType);
                // local decl
                for (const Stmt& stmt : body.stmts) {
                    if (std::holds_alternative<LocalDecl>(stmt.value)) {
                        addSumTypes(set, std::get<LocalDecl>(stmt.value).type);
                    }    
                }
            }
            return set;
        }

        // generate C
        void Source::generateC(ostream& stream) const {
            Map<Variant<
                const ProductType*, const SumType*, const Func*
            >, UUID> table;
            
            Set<ProductType> structs = getProductTypes();
            Set<SumType> unions = getSumTypes();

            for (const ProductType& struct_ : structs) {
                stream << "struct" << ' ' << table[&struct_] << '{';
                for (const Type& subtype : struct_.types) {
                    // qualifier
                    if (subtype.qualifier == Qualifier::MUT) {
                        // pass
                    }
                    else if (subtype.qualifier == Qualifier::CONST) {
                        stream << "const" << ' ';
                    }
                    // value
                    if (std::holds_alternative<Primitive>(subtype.value)) {
                        switch (std::get<Primitive>(subtype.value)) {
                        case Primitive::VOID:
                            stream << "void" << ' ';
                            break;
                        case Primitive::BOOL:
                            stream << "bool" << ' ';
                            break;
                        case Primitive::CHAR_UTF8:
                            stream << "char8" << ' ';
                            break;
                        case Primitive::INT8:
                            stream << "int8" << ' ';
                            break;
                        case Primitive::INT16:
                            stream << "int16" << ' ';
                            break;
                        case Primitive::INT32:
                            stream << "int32" << ' ';
                            break;
                        case Primitive::INT64:
                            stream << "int64" << ' ';
                            break;
                        case Primitive::UINT8:
                            stream << "uint8" << ' ';
                            break;
                        case Primitive::UINT16:
                            stream << "uint16" << ' ';
                            break;
                        case Primitive::UINT32:
                            stream << "uint32" << ' ';
                            break;
                        case Primitive::UINT64:
                            stream << "uint64" << ' ';
                            break;
                        case Primitive::FLOAT32:
                            stream << "float32" << ' ';
                            break;
                        case Primitive::FLOAT64:
                            stream << "float64" << ' ';
                            break;
                        default:
                            break;
                        }
                    }
                    else if (std::holds_alternative<ProductType>(subtype.value)) {
                        stream << table[&std::get<ProductType>(subtype.value)];
                    }
                    else if (std::holds_alternative<ProductType>(subtype.value)) {
                        stream << table[&std::get<SumType>(subtype.value)];
                    }

                    // category
                    if (subtype.category == Category::VAL) {
                        // pass
                    }
                    else if (subtype.category == Category::REF) {
                        stream << '*';
                    }
                }
                stream << '}' << ';';
            }
        }
    }
}