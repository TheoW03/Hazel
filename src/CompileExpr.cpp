#include <visitor.h>
#include <backend/CompilerUtil.h>

CompileExpr::CompileExpr(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::map<std::string, Function> func_map) : module(module), builder(builder), context(context)
{
    this->func_map = func_map;
}

llvm::Value *CompileExpr::Expression(std::shared_ptr<ASTNode> node)
{
    if (dynamic_cast<IntegerNode *>(node.get()))
    {
        auto c = dynamic_cast<IntegerNode *>(node.get());
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), c->number);
    }
    else if (dynamic_cast<DecimalNode *>(node.get()))
    {
        auto c = dynamic_cast<DecimalNode *>(node.get());
        return llvm::ConstantFP::get(context, llvm::APFloat(c->number));
    }
    else if (dynamic_cast<ExprNode *>(node.get()))
    {
        auto c = dynamic_cast<ExprNode *>(node.get());

        auto lhs = Expression(c->lhs);
        auto rhs = Expression(c->rhs);
        switch (c->operation.type)
        {
        case Addition:
            return builder.CreateAdd(lhs, rhs, "addition");
        case Multiplication:
            return builder.CreateMul(lhs, rhs, "addition");
        case Division:
            return builder.CreateSDiv(lhs, rhs, "addition");
        case Subtraction:
            return builder.CreateSub(lhs, rhs, "addition");
        case Modulas:
            return builder.CreateSRem(lhs, rhs, "modulas");
        case And:
            return builder.CreateAnd(lhs, rhs, "modulas");
        case Or:
            return builder.CreateOr(lhs, rhs, "modulas");
        case Left_Shift:
            return builder.CreateShl(lhs, rhs, "leftshoit");
        case Right_Shift:
            return builder.CreateLShr(lhs, rhs, "right_shift");
        default:

            break;
        }
    }
    return nullptr;
}
