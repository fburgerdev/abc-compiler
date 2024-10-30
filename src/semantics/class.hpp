#pragma once
#include "func.hpp"

namespace Compiler {
    namespace ABC {
        // Access
        enum class Access {
            PUBLIC, PRIVATE
        };
        // ClassContent
        template<class T>
        class ClassContent;

        // Class
        class Class : public Context {
        public:
            // constructor
            Class() = default;
            template<class Node>
            Class(const Node& node, Context* parent, Handler& handler);

            // findInCurrent
            virtual Var* findVarInCurrent(ID id, uint depth) override;
            virtual Func* findFuncInCurrent(ID id, uint depth) override;
            virtual Class* findClassInCurrent(ID id, uint depth) override;

            // name
            strview name;
            // classes, funcs, vars
            Map<string, ClassContent<Class>> classes;
            Map<string, ClassContent<Func>> funcs;
            Map<string, ClassContent<Var>> vars;
        };

        // ClassContent (Class, Func, Var)
        template<>
        class ClassContent<Class> {
        public:
            // constructor
            template<class... TArgs>
            ClassContent(Access access, TArgs&&... args)
                : access(access), cls(std::forward<TArgs>(args)...) {}
            
            // access, cls
            Access access;
            Class cls;
        };
        template<>
        class ClassContent<Func> {
        public:
            // constructor
            template<class... TArgs>
            ClassContent(Access access, TArgs&&... args)
                : access(access), func(std::forward<TArgs>(args)...) {}
            
            // access, func
            Access access;
            Func func;
        };
        template<>
        class ClassContent<Var> {
        public:
            // constructor
            template<class... TArgs>
            ClassContent(Access access, TArgs&&... args)
                : access(access), var(std::forward<TArgs>(args)...) {}
            
            // access, var
            Access access;
            Var var;
        };
    }
}