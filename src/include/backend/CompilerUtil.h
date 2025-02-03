#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <map>
#include <iostream>
#include <Frontend/Ast.h>
#include "llvm/IR/LLVMContext.h"

#ifndef TYPE_OF_EXPR_H
#define TYPE_OF_EXPR_H
enum TypeOfExpr
{
    Boolean_Type,
    Integer_Type,
    Float_Type,
    String_Type
};

#endif
#ifndef THUNKS_H
#define THUNKS_H
struct Thunks
{
    llvm::Type *thunk_type;
    llvm::Function *eval_func;
};
#endif
#ifndef COMPILED_FUNCTION_H
#define COMPILED_FUNCTION_H
struct Function
{
    llvm::Function *function;
    std::vector<Function> params;
    std::vector<Thunks> thunks;
};
#endif
#ifndef CONTEXT_H
#define CONTEXT_H

class CompilerContext
{
public:
    std::map<std::string, Function> func_map;
    std::map<std::string, llvm::Function *> CFunctions;
    std::map<std::string, llvm::Type *> types;
    std::vector<llvm::StructType *> lists;
    llvm::StructType *string_type;
    Function get_function(Tokens name);
    void add_function(Tokens name, Function f);
    void compile_cfunctions(llvm::Module &module, llvm::LLVMContext &context, llvm::IRBuilder<> &builder);
    llvm::StructType *get_string_type(llvm::LLVMContext &context, llvm::IRBuilder<> &builder);
    llvm::Type *compile_Type(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<Type> ty);
    llvm::FunctionType *compile_Function_Type(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<FunctionRefNode> n);
    Thunks get_thunk_types(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<FunctionRefNode> n);
};
#endif
// llvm::Type *compileType(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<Type> ty, CompilerContext &ctx);
// llvm::FunctionType *CompileFunctionType(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<FunctionRefNode> n, CompilerContext &ctx);

TypeOfExpr get_expr_type(std::shared_ptr<ASTNode> n);
TypeOfExpr get_bool_expr_type(std::shared_ptr<ASTNode> n);