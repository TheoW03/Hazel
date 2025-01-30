#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <Frontend/Ast.h>
#include "llvm/IR/LLVMContext.h"
#include <memory>
#include <backend/CompilerUtil.h>

llvm::Type *compileType(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<Type> ty, CompilerContext &ctx)
{

    // TODO:
    // very temporary solution. need to fix later to include functions
    if (dynamic_cast<NativeType *>(ty.get()))
    {
        auto p = dynamic_cast<NativeType *>(ty.get());
        if (p->type.type == TokenType::Integer || p->type.type == TokenType::Uinteger)
        {
            return builder.getInt64Ty();
        }
        else if (p->type.type == TokenType::Decimal)
        {
            return builder.getDoubleTy();
        }
        else if (p->type.type == TokenType::Byte || p->type.type == TokenType::character || p->type.type == TokenType::Ubyte)
        {
            return builder.getInt8Ty();
        }
        else if (p->type.type == TokenType::boolean)
        {
            return builder.getInt1Ty();
        }
        else if (p->type.type == TokenType::string)
        {
            return ctx.get_string_type(context, builder);
        }
    }
    // NOTE:
    //  for lists generate the entire list and when index simply shrink down the list
    //  and return the value indexed
    else if (dynamic_cast<ListType *>(ty.get()))
    {
        auto p = dynamic_cast<ListType *>(ty.get());

        auto c = compileType(builder, context, p->inner_type, ctx);
        for (int i = 0; i < ctx.lists.size(); i++)
        {
            if (ctx.lists[i]->getElementType(0) == c)
            {
                std::cout << "aaa" << std::endl;
                return llvm::PointerType::getUnqual(ctx.lists[i]);
            }
        }
        llvm::StructType *nodeType = llvm::StructType::create(context, "Node");
        std::vector<llvm::Type *> elements = {c, llvm::PointerType::getUnqual(nodeType)};
        nodeType->setBody(elements);

        ctx.lists.push_back((nodeType));
        return llvm::PointerType::getUnqual(nodeType);
    }
    return builder.getVoidTy();
}

llvm::FunctionType *CompileFunctionType(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<FunctionRefNode> n, CompilerContext &ctx)
{
    auto c = n->RetType;
    std::vector<llvm::Type *> a;
    for (int i = 0; i < n->params.size(); i++)
    {
        auto funct = CompileFunctionType(builder, context, n->params[i], ctx);
        a.push_back(llvm::PointerType::getUnqual(funct));
    }
    llvm::FunctionType *functype = llvm::FunctionType::get(
        compileType(builder, context, c, ctx), a, false);
    return functype;
}
TypeOfExpr get_bool_expr_type(std::shared_ptr<ASTNode> n)
{
    auto c = dynamic_cast<BooleanExprNode *>(n.get());
    if (dynamic_cast<IntegerNode *>(c->lhs.get()) && dynamic_cast<IntegerNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    else if (dynamic_cast<DecimalNode *>(c->lhs.get()) && dynamic_cast<DecimalNode *>(c->rhs.get()))
        return TypeOfExpr::Float_Type;
    else if (dynamic_cast<BooleanConstNode *>(c->lhs.get()) && dynamic_cast<BooleanConstNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    if (dynamic_cast<BooleanExprNode *>(c->lhs.get()))
        return get_bool_expr_type(c->lhs);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs);
}
TypeOfExpr get_expr_type(std::shared_ptr<ASTNode> n)
{
    auto c = dynamic_cast<ExprNode *>(n.get());
    if (dynamic_cast<IntegerNode *>(c->lhs.get()) && dynamic_cast<IntegerNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    else if (dynamic_cast<DecimalNode *>(c->lhs.get()) && dynamic_cast<DecimalNode *>(c->rhs.get()))
        return TypeOfExpr::Float_Type;
    else if (dynamic_cast<BooleanConstNode *>(c->lhs.get()) && dynamic_cast<BooleanConstNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    if (dynamic_cast<ExprNode *>(c->lhs.get()))
        return get_expr_type(c->lhs);
    if (dynamic_cast<ExprNode *>(c->rhs.get()))
        return get_expr_type(c->rhs);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs);
    if (dynamic_cast<BooleanExprNode *>(c->lhs.get()))
        return get_bool_expr_type(c->lhs);
}
