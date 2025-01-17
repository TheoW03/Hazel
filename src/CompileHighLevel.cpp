#include <visitor.h>
#include <backend/CompilerUtil.h>

CompileHighLevel::CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context) : module(module), builder(builder), context(context)
{
    // this->module = module;
    // this->builder = builder;
    // this->context = context;
}

void CompileHighLevel::Visit(ASTNode *node)
{
}
void CompileHighLevel::Visit(FunctionNode *node)
{
    using std::make_pair;
    // auto c = node->f->RetType;
    // llvm::FunctionType *functype = llvm::FunctionType::get(
    //     compileType(builder, c), {}, false);

    // llvm::Function *function = llvm::Function::Create(
    //     functype, llvm::Function::ExternalLinkage, node->f->FunctionName.value, module);
    // func_map.insert(, node->f->FunctionName.value, );
    func_map.insert(make_pair(node->f->FunctionName.value, CompileFunctionHeader(node->f)));
}

void CompileHighLevel::Visit(ReturnNode *node)
{
}

Function CompileHighLevel::CompileFunctionHeader(std::shared_ptr<FunctionRefNode> n)
{
    auto c = n->RetType;

    std::vector<Function> f;
    // std::vector<llvm::Type *> a;
    // for (int i = 0; i < n->params.size(); i++)
    // {
    //     auto c = CompileFunctionHeader(n->params[i]);
    //     f.push_back(c);
    //     a.push_back(c.function->getType());
    // }
    llvm::FunctionType *functype = CompileFunctionType(builder, n);
    llvm::Function *function = llvm::Function::Create(
        functype, llvm::Function::ExternalLinkage, n->FunctionName.value, module);
    return {function, f};
}

std::map<std::string, Function> CompileHighLevel::get_map()
{
    return this->func_map;
}
