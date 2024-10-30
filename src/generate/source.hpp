#pragma once
#include "common.hpp"

namespace Compiler {
    namespace IR {
        // forward: Type
        struct Type;
        // ProductType
        struct ProductType {
            // compare
            bool operator<(const ProductType& other) const;

            // types
            List<Type> types;
        };
        // SumType
        struct SumType {
            // compare
            bool operator<(const SumType& other) const;

            // types
            List<Type> types;
        };

        // Qualifier
        enum class Qualifier {
            MUT, CONST
        };
        // Primitive
        enum class Primitive {
            VOID, BOOL, CHAR_UTF8,
            INT8, INT16, INT32, INT64,
            UINT8, UINT16, UINT32, UINT64,
            FLOAT32, FLOAT64
        };
        // Category
        enum class Category {
            VAL, REF
        };
        // Type
        struct Type {
            // compare
            bool operator<(const Type& other) const;

            // qualifier, value, category
            Qualifier qualifier = Qualifier::MUT;
            Variant<
                Primitive,
                ProductType, SumType
            > value = Primitive::VOID;
            Category category = Category::VAL;
        };

        // Func
        struct Func {
            // name, params, returnType
            string name;
            List<Type> params;
            Type returnType;
        };

        //* Expr
        // forward: Expr
        struct Expr;
        // Literal
        struct Literal {
            // value
            Variant<
                bool, char,
                int8, int16, int32, int64,
                uint8, uint16, uint32, uint64,
                float32, float64
            > value = false;
        };
        // VarAccess
        struct VarAccess {
            // bound, members
            Type* bound = nullptr;
            List<Type*> members;
        };
        // FuncCall
        struct FuncCall {
            // bound, args
            Func* bound = nullptr;
            List<Expr> args;
        };
        // Expr
        struct Expr {
            // value
            Variant<
                Literal, VarAccess, FuncCall
            > value = Literal();
        };

        //* Stmt
        // forward: Stmt
        struct Stmt;
        // CodeBlock
        struct CodeBlock {
            // stmts
            List<Stmt> stmts;
        };
        // LocalDecl
        struct LocalDecl {
            // type, init
            Type type;
            Opt<FuncCall> init;
        };
        // Cond
        struct Cond {
            // ifCond, elifConds, elseBlock
            Pair<Expr, CodeBlock> ifCond;
            List<Pair<Expr, CodeBlock>> elifConds;
            Opt<CodeBlock> elseBlock;
        };
        // Loop
        struct Loop {
            // expr / block
            Expr expr;
            CodeBlock block;
        };
        // Stmt
        struct Stmt {
            // value
            Variant<
                LocalDecl, Expr,
                Cond, Loop
            > value = LocalDecl();
        };

        // Source
        struct Source {
            // getProductTypes
            Set<ProductType> getProductTypes() const;
            // getSumTypes
            Set<SumType> getSumTypes() const;

            // generate C
            void generateC(ostream& stream) const;

            // funcs
            List<Pair<Func, CodeBlock>> funcs;
        };
    }
}