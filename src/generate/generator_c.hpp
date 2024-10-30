#pragma once
#include "source.hpp"
#include "uuid.hpp"

namespace Compiler {
    // GeneratorC
    class GeneratorC {
    public:
        // constructor
        GeneratorC(ostream& stream);

        // generate
        // :: generic
        template<typename... Args>
        void generate(Variant<Args...> variant) {
            std::visit([&] (const auto& value) {
                generate(value);
            }, variant);
        }
        // :: type
        void generate(IR::Primitive primitive);
        void generate(const IR::Type& type);
        // :: expr
        void generate(const IR::Literal& literal);
        void generate(const IR::VarAccess& varAccess);
        void generate(const IR::FuncCall& funcCall);
        void generate(const IR::Expr& expr);
        // :: stmt
        void generate(const IR::LocalDecl& localDecl);
        void generate(const IR::Cond& cond);
        void generate(const IR::Loop& loop);
        void generate(const IR::Stmt& stmt);
        void generate(const IR::CodeBlock& block);
        // :: product
        void generate(const IR::ProductType& product);
        // :: sum
        void generate(const IR::SumType& sum);
        // :: func
        void generate(const IR::Func& funcHead);
        // :: source
        void generate(const IR::Source& source);
    private:
        ostream& _stream;
        Map<Variant<
            const IR::ProductType*, const IR::SumType*,
            const IR::Type*, const IR::Func*
        >, UUID> _table;
    };
}