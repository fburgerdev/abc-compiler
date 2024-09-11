#pragma once
#include "func.hpp"

namespace Compiler {
    // Access
    enum class Access {
        PUBLIC, PRIVATE
    };

    // Class
    struct Class : public Scope {
        // constructor
        Class() = default;
        template<class Node>
        Class(const Node& node, Handler& handler);

        // setScope
        void setScope(Scope* scope);
        // "Scope" implementation
        virtual Class* searchClass(const Identifier& id) override;
        virtual Func* searchFunc(const Identifier& id, const List<Type>& argTypes) override;
        virtual Var* searchVar(const Identifier& id) override;
        // dependsOn
        bool dependsOn(const Class& cls) const;        

        // name
        strview name;
        // member
        List<Pair<Access, InitFunc>> initFuncs;
        List<Pair<Access, MemberFunc>> memberFuncs;
        List<Pair<Access, MemberVar>> memberVars;
        // static
        List<Pair<Access, StaticFunc>> staticFuncs;
        List<Pair<Access, StaticVar>> staticVars;
        List<Pair<Access, Class>> classes;
    };
}