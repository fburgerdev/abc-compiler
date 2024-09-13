#pragma once
#include "common.hpp"

namespace Compiler {
    namespace CPP {
        // API
        struct API {
            // generate
            virtual void generate(ostream& stream) const = 0;
        };

        // forward
        class Class;

        // Primitive
        enum class Primitive {
            VOID, BOOL, CHAR, INT, UINT, FLOAT, DOUBLE,
        };
        // Type
        struct Type : API {
            // constructor
            template<class ABCType>
            Type(const ABCType& abcType);

            // Qualifier
            enum class Qualifier {
                NONE, CONST,
            };
            // ValueCategory
            enum class ValueCategory {
                VALUE, LVALUE, RVALUE,
            };

            // generate
            virtual void generate(ostream& stream) const override;

            // qualifier / category / value
            Qualifier qualifier = Qualifier::NONE;
            ValueCategory valueCategory = ValueCategory::VALUE;
            Variant<Primitive, const Class*> value = nullptr;
        };

        // Context
        struct Context {
            // constructor
            Context() = default;
            Context(const Context& other, const string& name)
                : namespaces(other.namespaces) {
                namespaces.emplace_back(name);
            }

            // namespaces
            List<string> namespaces;
        };

        // Var
        struct Var : API {
            // constructor
            template<class ABCVar>
            Var(const ABCVar& abcVar, const Context& context = {});

            // Specifier
            enum class Specifier {
                NONE, STATIC, EXTERN, STATIC_INLINE,
            };
            
            // generate
            virtual void generate(ostream& stream) const override;

            // specifier / name / type
            Specifier specifier = Specifier::NONE;
            string name;
            Type type;
            Context context;
        };

        // Func
        struct Func : API {
            // constructor
            template<class ABCFunc>
            Func(const ABCFunc& abcFunc, const Context& context = {});

            // Storage
            enum class Storage {
                NONE, STATIC, EXTERN,
            };

            // generate
            virtual void generate(ostream& stream) const override;

            // storage / name / params / isConst / returnType
            Storage storage = Storage::NONE;
            string name;
            List<Var> params;
            bool isConst = false;
            Opt<Type> returnType;
            Context context;
        };

        // Access
        enum class Access {
            PUBLIC, PRIVATE
        };
        // Class
        struct Class : API {
            // constructor
            template<class ABCClass>
            Class(const ABCClass& abcClass, const Context& context = {});

            // generate
            virtual void generate(ostream& stream) const override;

            // name / content / order
            string name;
            List<Pair<Access, Class>> classes;
            List<Pair<Access, Func>> initFuncs;
            List<Pair<Access, Func>> staticFuncs;
            List<Pair<Access, Func>> memberFuncs;
            List<Pair<Access, Var>> staticVars;
            List<Pair<Access, Var>> memberVars;
            List<Pair<Access, API*>> order;
            Context context;
        };

        // Global
        struct Global : API {
            // constructor
            template<class Group>
            Global(const Group& group);
            // addGroup
            template<class Group>
            void addGroup(const Group& group, const Context& context);

            // generate
            virtual void generate(ostream& stream) const override;

            // content / order
            List<Class> classes;
            List<Func> funcs;
            List<Var> vars;
            List<API*> order;
        };
    }
}