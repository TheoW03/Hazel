#include <Frontend/Ast.h>
#include <visitor.h>
#include <map>

ASTNode::~ASTNode()
{
}
ASTNode::ASTNode()
{
}
IntegerNode::IntegerNode(Tokens num)
{
    if (num.type == TokenType::BaseTenDigit)
    {
        this->number = stoi(num.value);
    }
    else if (num.type == TokenType::HexDigit)
    {
        this->number = std::stoul(num.value, nullptr, 16);
    }
    else if (num.type == TokenType::BinaryDigit)
    {
        this->number = std::stoul(num.value, nullptr, 2);
    }
}
void IntegerNode::Accept(Visitor *v) {}

std::string IntegerNode::to_string()
{
    return std::to_string(this->number);
}
ExprNode::ExprNode(std::shared_ptr<ASTNode> lhs, Tokens operation, std::shared_ptr<ASTNode> rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->operation = operation;
}
void ExprNode::Accept(Visitor *v)
{
}

std::string ExprNode::to_string()
{
    std::map<TokenType, std::string> token_map;

    token_map[TokenType::Addition] = "+";
    token_map[TokenType::Subtraction] = "-";
    token_map[TokenType::Multiplication] = "*";
    token_map[TokenType::Division] = "/";
    return lhs->to_string() + token_map[operation.type] + rhs->to_string();
}

FunctionRefNode::FunctionRefNode(Tokens name,
                                 std::vector<std::shared_ptr<FunctionRefNode>> params,
                                 std::shared_ptr<Type> returnType)
{
    this->FunctionName = name;
    this->RetType = returnType;
    this->params = params;
}

void FunctionRefNode::Accept(Visitor *v)
{
    // std::shared_ptr<FunctionNode> sharedPtr(this);
    v->Visit(this);
    // v.Visit(std::make_shared<FunctionNode>(this));
}

std::string FunctionRefNode::to_string()
{
    return std::string();
}

FunctionNode::FunctionNode(std::shared_ptr<FunctionRefNode> functionHeader, std::vector<std::shared_ptr<ASTNode>> stmnts)
{
    this->f = functionHeader;
    this->stmnts = stmnts;
}

void FunctionNode::Accept(Visitor *v)
{
    v->Visit(this);
    // v.Visit(std::make_shared<FunctionNode>(this));
}

std::string FunctionNode::to_string()
{
    return std::string();
}

std::string NativeType::to_string()
{
    return std::string();
}
Type::Type() {}
Type::~Type() {}
FunctionType::FunctionType(std::vector<std::shared_ptr<Type>> params,
                           std::shared_ptr<Type> returnType)
{
}
NativeType::NativeType(Tokens type)
{
    this->type = type;
}
std::string FunctionType::to_string()
{
    return std::string();
}

std::string Type::to_string()
{
    return "";
}

NoneNode::NoneNode()
{
}

void NoneNode::Accept(Visitor *v)
{
    v->Visit(this);
}

std::string NoneNode::to_string()
{
    return std::string();
}

ReturnNode::ReturnNode(std::shared_ptr<ASTNode> expr)
{
    this->Expr = expr;
}

void ReturnNode::Accept(Visitor *v)
{
    v->Visit(this);
}

std::string ReturnNode::to_string()
{
    return std::string();
}

ListType::ListType(std::shared_ptr<Type> inner)
{
}

std::string ListType::to_string()
{
    return std::string();
}

ModuleNode::ModuleNode(std::vector<std::shared_ptr<ASTNode>> functions, Tokens name)
{
    this->functions = functions;
    this->name = name;
}

void ModuleNode::Accept(Visitor *v)
{
}

std::string ModuleNode::to_string()
{
    return std::string();
}
