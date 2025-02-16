#include <backend/CompilerUtil.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
Function CompilerContext::get_function(Tokens name)
{
    return func_map[name.value];
}
void CompilerContext::add_function(Tokens name, Function f)
{
    this->func_map.insert(std::make_pair(name.value, f));
}
void CompilerContext::compile_cfunctions(llvm::Module &module, llvm::LLVMContext &context, llvm::IRBuilder<> &builder)
{
    auto snprintftype = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy(), builder.getInt64Ty(), builder.getInt8PtrTy()}, true);
    auto snprintffunc = llvm::Function::Create(
        snprintftype, llvm::Function::ExternalLinkage, "snprintf", module);
    this->CFunctions.insert(std::make_pair("snprintf", snprintffunc));
    auto printftype = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy()}, true);
    auto printf_func = llvm::Function::Create(
        printftype, llvm::Function::ExternalLinkage, "printf", module);
    this->CFunctions.insert(std::make_pair("printf", printf_func));

    auto strncpy_type = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy(), builder.getInt8PtrTy(), builder.getInt64Ty()}, false);
    auto strncpy_func = llvm::Function::Create(
        strncpy_type, llvm::Function::ExternalLinkage, "strncpy", module);
    this->CFunctions.insert(std::make_pair("strncpy", strncpy_func));

    // this->CFunctions["snprintf"] = snprintf;

    // llvm::Function *function = llvm::Function::Create( builder.getInt64Ty().
    // functype, llvm::Function::ExternalLinkage, n->FunctionName.value, module);
}
llvm::StructType *CompilerContext::get_string_type(llvm::LLVMContext &context, llvm::IRBuilder<> &builder)
{
    if (this->string_type == nullptr)
    {

        this->string_type = llvm::StructType::create(context, "string");
        std::vector<llvm::Type *> elements = {builder.getInt8PtrTy(), builder.getInt64Ty()};
        this->string_type->setBody(elements);

        // uint64_t StructSize = DL.getTypeAllocSize(MyStruct);
    }
    return string_type;
}
llvm::Type *CompilerContext::compile_Type(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<Type> ty)
{

    //     // TODO:
    //     // very temporary solution. need to fix later to include functions
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
            return this->get_string_type(context, builder);
        }
    }
    // NOTE:
    //  for lists generate the entire list and when index simply shrink down the list
    //  and return the value indexed
    else if (dynamic_cast<ListType *>(ty.get()))
    {
        auto p = dynamic_cast<ListType *>(ty.get());

        auto c = this->compile_Type(builder, context, p->inner_type);
        for (int i = 0; i < this->lists.size(); i++)
        {
            if (this->lists[i]->getElementType(0) == c)
            {
                std::cout << "aaa" << std::endl;
                return llvm::PointerType::getUnqual(this->lists[i]);
            }
        }
        llvm::StructType *nodeType = llvm::StructType::create(context, "Node");
        std::vector<llvm::Type *> elements = {c, llvm::PointerType::getUnqual(nodeType)};
        nodeType->setBody(elements);

        this->lists.push_back((nodeType));
        return llvm::PointerType::getUnqual(nodeType);
    }
    return builder.getVoidTy();
}

llvm::FunctionType *CompilerContext::compile_Function_Type(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<FunctionRefNode> n)
{
    auto c = n->RetType;
    std::vector<llvm::Type *> a;
    for (int i = 0; i < n->params.size(); i++)
    {
        auto funct = compile_Function_Type(builder, context, n->params[i]);
        a.push_back(get_thunk_types(builder, context, n->params[i]).thunk_type);
    }
    llvm::FunctionType *functype = llvm::FunctionType::get(
        compile_Type(builder, context, c), a, false);
    return functype;
}

Thunks CompilerContext::get_thunk_types(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<FunctionRefNode> n)
{

    // std::vector<Thunks> thunks;
    // this->string_type = llvm::StructType::create(context, "Thunk");
    auto thunk = llvm::StructType::create(context, "Thunk");
    auto funct = compile_Function_Type(builder, context, n);
    std::vector<llvm::Type *> elements = {compile_Type(builder, context, n->RetType), llvm::PointerType::getUnqual(funct), builder.getInt1Ty()};
    // this->string_type->setBody(elements);
    thunk->setBody(elements);
    return {thunk, nullptr};
}
