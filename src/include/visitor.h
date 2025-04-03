

#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <backend/Scope.h>
#include <Frontend/Ast.h>
#include <map>
#include <memory>
#include <backend/CompilerContext.h>
#include <backend/CompilerUtil.h>

#ifndef SEMANTUC_FUCN
#define SEMANTUC_FUCN
struct SemanticFunction
{
    std::vector<std::shared_ptr<Type>> paremeters;
    std::shared_ptr<Type> returnType;
};

#endif

#ifndef SEMANTIC_MODULE_H
#define SEMANTIC_MODULE_H

struct SemanticModule
{
    std::set<std::string> functions;
    std::set<std::string> exported_functions;
    std::vector<Tokens> imports;
    // std::shared_ptr<ModuleNode> module;
    // std::map<std::string, std::vector<SemanticFunction>> functions;
};

#endif

#ifndef SEMANTIC_SCOPE
#define SEMANTIC_SCOPE
class Scope
{
public:
    virtual void add_function(FunctionNode *f) = 0;
    virtual SemanticFunction get_function() = 0;
};
#endif

#ifndef GLOBAL_SCOPE_H
#define GLOBAL_SCOPE_H
class GlobalScope : public Scope
{
public:
    std::map<std::string, std::vector<SemanticFunction>> functions;
    std::map<std::string, std::vector<SemanticFunction>> exported_functions;
    void add_function(FunctionNode *f) override;
    SemanticFunction get_function() override;
};
#endif

#ifndef VISITOR_H
#define VISITOR_H
class Visitor
{
public:
    Visitor();
    ~Visitor();
    virtual void Visit(ASTNode *node);
    virtual void Visit(ModuleNode *node);
    virtual void Visit(FunctionNode *node);
    virtual void Visit(ReturnNode *node);
    virtual void Visit(FunctionCallNode *node);
    virtual void Visit(ProgramNode *node);
    virtual void Visit(IntegerNode *node);
    virtual void Visit(DecimalNode *node);
    virtual void Visit(CharNode *node);
    virtual void Visit(StringNode *node);
    virtual void Visit(BooleanExprNode *node);
    virtual void Visit(BooleanConstNode *node);
    virtual void Visit(ExprNode *node);
    virtual void Visit(ListNode *node);
};
#endif

// #ifndef FOLD_EXPR_H
// #define FOLD_EXPR_H
// class FoldExpr
// {
// private:
//     std::shared_ptr<ASTNode> fold_integer(std::shared_ptr<IntegerNode> lhs, Tokens op, std::shared_ptr<IntegerNode> rhs);
//     std::shared_ptr<ASTNode> fold_decimal(std::shared_ptr<DecimalNode> lhs, Tokens op, std::shared_ptr<DecimalNode> rhs);
//     std::shared_ptr<ASTNode> fold_string(std::shared_ptr<StringNode> lhs, Tokens op, std::shared_ptr<StringNode> rhs);

// public:
//     FoldExpr();
//     std::shared_ptr<ASTNode> fold_expr(std::shared_ptr<ASTNode> n);
// };

// #endif

// #ifndef SEMANTIC_H
// #define SEMANTIC_H
// class ConstantFoldingVisitor : public Visitor
// {
// public:
//     ConstantFoldingVisitor();
//     void Visit(ASTNode *node) override;
//     void Visit(FunctionNode *node) override;
//     void Visit(ModuleNode *node) override;
//     void Visit(ReturnNode *node) override;
//     void Visit(FunctionCallNode *node) override;
//     void Visit(ProgramNode *node) override;
// };
// #endif

// #ifndef SEMANTIC_TOP_H
// #define SEMANTIC_TOP_H
// class SemanticGlobalScopeVisitor : public Visitor
// {
// public:
//     std::map<std::string, std::shared_ptr<ModuleNode>> avail_modules;
//     std::set<std::string> module_functions;
//     std::set<std::string> exported_functions;
//     std::map<std::string, SemanticModule> modules;
//     std::shared_ptr<ModuleNode> current;
//     SemanticModule current_AST_module;
//     void Visit(ASTNode *node) override;
//     void Visit(FunctionNode *node) override;
//     void Visit(ModuleNode *node) override;
//     void Visit(ReturnNode *node) override;
//     void Visit(FunctionCallNode *node) override;
//     void Visit(ProgramNode *node) override;
// };
// #endif

// #ifndef FUNCTION_LOCAL_SCOPE_H
// #define FUNCTION_LOCAL_SCOPE_H
// struct FunctionLocalScope
// {
//     std::set<std::string> functions;
// };
// #endif

// #ifndef SEMANTIC_SCOPE_VISTOR_H
// #define SEMANTIC_SCOPE_VISTOR_H

// class SemanticLocalScopeVisitor : public Visitor
// {
// private:
//     std::vector<FunctionLocalScope> scope;
//     std::optional<int> find_function_global(Tokens name);
//     std::optional<int> find_function_local(Tokens name);
//     std::map<std::string, SemanticModule> modules;
//     SemanticModule current_AST_module;

// public:
//     SemanticLocalScopeVisitor(std::map<std::string, SemanticModule> modules);

//     std::optional<int> find_function(Tokens name);
//     void Visit(ASTNode *node) override;
//     void Visit(FunctionNode *node) override;
//     void Visit(ModuleNode *node) override;
//     void Visit(ReturnNode *node) override;
//     void Visit(FunctionCallNode *node) override;
//     void Visit(ProgramNode *node) override;
//     void Visit(ExprNode *node) override;
// };
// #endif

#ifndef COMPILER_H
#define COMPILER_H
class CompileHighLevel : public Visitor
{
private:
    std::map<std::string, CompiledModuleClass> modules;

public:
    llvm::Module &module;
    std::vector<std::shared_ptr<ASTNode>> functions;

    CompilerContext compiler_context;
    std::map<std::string, Function> func_map;
    std::map<std::string, Function> exported_func_map;
    std::queue<Function> compiled_functions;
    bool is_global;

    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context);
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;

    void Visit(ReturnNode *node) override;
    void Visit(FunctionCallNode *node) override;
    void Visit(ProgramNode *node) override;

    Function CompileFunctionHeader(std::shared_ptr<FunctionRefNode> n);
    ProgramScope getProgramScope();
};

#endif

#ifndef COMPILE_STATEMENT_H
#define COMPILE_STATEMENT_H
class CompileStatement : public Visitor
{
public:
    llvm::Module &module;
    CompiledModule current_module;
    CompilerContext compiler_context;
    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    ProgramScope program_scope;
    CompileStatement(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, CompilerContext compiler_context);
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;

    void Visit(ReturnNode *node) override;
    void Visit(ProgramNode *node) override;
    void Visit(FunctionCallNode *node) override;
};

#endif

#ifndef COMPILE_EXPR_H
#define COMPILE_EXPR_H
class CompileExpr
{
private:
    llvm::Value *CompileStr(llvm::Value *str, llvm::Value *length, llvm::Value *structure);
    llvm::Value *IntegerMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *FloatMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *StringMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs);

    llvm::Value *IntegerBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *FloatBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs);

public:
    llvm::Module &module;
    ProgramScope program;
    // std::map<std::string, Function> func_map;
    CompilerContext compiler_context;
    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    CompileExpr(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context,
                CompilerContext compiler_context,
                ProgramScope program);
    llvm::Value *IntMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *FloatMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *BoolIntMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *BoolFloatMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *StringMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *Expression(std::shared_ptr<ASTNode> node);
};
#endif
