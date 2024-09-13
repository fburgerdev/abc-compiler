#include "semantics/code/source.hpp"
#include "generate/source.hpp"

namespace Compiler {
    namespace CPP {
        // ABC
        using ABCSource = Compiler::Source;
        using ABCClass = Compiler::Class;
        using ABCType = Compiler::Type;

        // classMap
        static Map<const Compiler::Class*, const Class*> s_classMap;

        // Type
        template<>
        Type::Type<ABCType>(const ABCType& abc) {
            if (abc.isPrimitive()) {
                switch (abc.getPrimitive()) {
                    case Compiler::Primitive::VOID:
                        value = Primitive::VOID;
                        break;
                    case Compiler::Primitive::BOOL:
                        value = Primitive::BOOL;
                        break;
                    case Compiler::Primitive::CHAR:
                        value = Primitive::CHAR;
                        break;
                    case Compiler::Primitive::INT:
                        value = Primitive::INT;
                        break;
                    case Compiler::Primitive::UINT:
                        value = Primitive::UINT;
                        break;
                    case Compiler::Primitive::FLOAT:
                        value = Primitive::FLOAT;
                        break;
                    default:
                        throw std::runtime_error("unknown primitive");
                }
            }
            else if (abc.isClass()){
                value = s_classMap.at(&abc.getClass());
            }
        }

        // Var
        template<>
        Var::Var<StaticVar>(const StaticVar& abc, const Context& context)
            : name(abc.name), type(Type(abc.getType())), context(context) {
            //TODO: find more elegant way for storage
            if (dynamic_cast<const Class*>(abc.scope)) {
                specifier = Specifier::STATIC_INLINE;
            }
            else {
                specifier = Specifier::STATIC;
            }
        }
        template<>
        Var::Var<MemberVar>(const MemberVar& abc, const Context& context)
            : name(abc.name), type(Type(abc.getType())), context(context) {}
        template<>
        Var::Var<FuncVar>(const FuncVar& abc, const Context& context)
            : name(abc.name), type(Type(abc.getType())), context(context) {}
        template<>
        Var::Var<LocalVar>(const LocalVar& abc, const Context& context)
            : name(abc.name), type(Type(abc.getType())), context(context) {}

        // Func
        template<>
        Func::Func<InitFunc>(const InitFunc& abc, const Context& context)
            : context(context) {
            if (const auto* cls = dynamic_cast<const Compiler::Class*>(abc.parentScope)) {
                name = cls->name;
            }
        }
        template<>
        Func::Func<StaticFunc>(const StaticFunc& abc, const Context& context)
            : storage(Storage::STATIC), returnType(Type(abc.returnType)), context(context) {
            name = (abc.isOperator() ? "operator" + string(abc.name) : string(abc.name));
            for (const auto& param : abc.params) {
                params.emplace_back(param, Context());
            }
        }
        template<>
        Func::Func<MemberFunc>(const MemberFunc& abc, const Context& context)
            : isConst(abc.isConst), returnType(Type(abc.returnType)), context(context) {
            name = (abc.isOperator() ? "operator" + string(abc.name) : string(abc.name));
            for (const auto& param : abc.params) {
                params.emplace_back(param, Context());
            }
        }

        // Class
        template<>
        Class::Class<ABCClass>(const ABCClass& abc, const Context& context)
            : context(context) {
            s_classMap.emplace(&abc, this);
            name = abc.name;
            Context childContext(context, name);
            classes.reserve(abc.classes.size());
            for (const auto& [abcAccess, child] : abc.classes) {
                Access access = (abcAccess == Compiler::Access::PUBLIC ? Access::PUBLIC : Access::PRIVATE);
                classes.emplace_back(access, child);
                order.emplace_back(access, &classes.back().second); //TODO: proper order
            }
            initFuncs.reserve(abc.initFuncs.size());
            for (const auto& [abcAccess, child] : abc.initFuncs) {
                Access access = (abcAccess == Compiler::Access::PUBLIC ? Access::PUBLIC : Access::PRIVATE);
                initFuncs.emplace_back(access, child);
                order.emplace_back(access, &initFuncs.back().second); //TODO: proper order
            }
            staticFuncs.reserve(abc.staticFuncs.size());
            for (const auto& [abcAccess, child] : abc.staticFuncs) {
                Access access = (abcAccess == Compiler::Access::PUBLIC ? Access::PUBLIC : Access::PRIVATE);
                staticFuncs.emplace_back(access, child);
                order.emplace_back(access, &staticFuncs.back().second); //TODO: proper order
            }
            memberFuncs.reserve(abc.memberFuncs.size());
            for (const auto& [abcAccess, child] : abc.memberFuncs) {
                Access access = (abcAccess == Compiler::Access::PUBLIC ? Access::PUBLIC : Access::PRIVATE);
                memberFuncs.emplace_back(access, child);
                order.emplace_back(access, &memberFuncs.back().second); //TODO: proper order
            }
            staticVars.reserve(abc.staticVars.size());
            for (const auto& [abcAccess, child] : abc.staticVars) {
                Access access = (abcAccess == Compiler::Access::PUBLIC ? Access::PUBLIC : Access::PRIVATE);
                staticVars.emplace_back(access, child);
                order.emplace_back(access, &staticVars.back().second); //TODO: proper order
            }
            memberVars.reserve(abc.memberVars.size());
            for (const auto& [abcAccess, child] : abc.memberVars) {
                Access access = (abcAccess == Compiler::Access::PUBLIC ? Access::PUBLIC : Access::PRIVATE);
                memberVars.emplace_back(access, child);
                order.emplace_back(access, &memberVars.back().second); //TODO: proper order
            }
        }

        // addGroup
        template<>
        void Global::addGroup<Group>(const Group& abc, const Context& context) {
            for (const auto& [name, child] : abc.groups) {
                addGroup(child, Context(context, name));
            }
            for (const auto& [name, child] : abc.classes) {
                classes.emplace_back(child, context);
            }
            for (const auto& [name, child] : abc.funcs) {
                for (const auto& grand_child : child) {
                    funcs.emplace_back(grand_child, context);
                }
            }
            for (const auto& [name, child] : abc.vars) {
                vars.emplace_back(child, context);
            }
        }
        // Global
        template<>
        Global::Global<Group>(const Group& abc) {
            addGroup(abc, {});
            for (auto& child : classes) {
                order.emplace_back(&child); //TODO: proper order
            }
            for (auto& child : funcs) {
                order.emplace_back(&child);
            }
            for (auto& child : vars) {
                order.emplace_back(&child);
            }
        }

        // Source
        template<>
        Source::Source<ABCSource>(const ABCSource& abc)
            : global(abc.global) {}
    }
}