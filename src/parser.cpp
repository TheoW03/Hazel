#include <vector>
#include <optional>
#include <Frontend/lexxer.h>
#include <iostream>
#include <Frontend/Ast.h>
std::optional<Tokens> current;

std::optional<std::shared_ptr<ASTNode>> expression(std::vector<Tokens> &tokens);
std::optional<std::shared_ptr<FunctionRefNode>> parse_function_ref(std::vector<Tokens> &tokens);

std::optional<Tokens> match_and_remove(TokenType token_type, std::vector<Tokens> &tokens)
{
    if (tokens.empty())
        return {};
    if (tokens[0].type == token_type)
    {
        current = tokens[0];
        tokens.erase(tokens.begin());
        return current;
    }
    return {};
}
std::optional<Tokens> match_and_remove(std::vector<TokenType> token, std::vector<Tokens> &tokens)
{
    for (int i = 0; i < token.size(); i++)
    {
        if (tokens[0].type == token[i])
        {
            current = tokens[0];
            tokens.erase(tokens.begin());
            return current;
        }
    }
    return {};
}

bool look_ahead(TokenType t, std::vector<Tokens> &tokens)
{
    return t == tokens[0].type;
}

bool look_ahead(std::vector<TokenType> token, std::vector<Tokens> &tokens)
{
    for (int i = 0; i < token.size(); i++)
    {
        if (token[i] == tokens[0].type)
        {
            return true;
        }
    }
    return false;
}
std::optional<std::shared_ptr<ASTNode>> factor(std::vector<Tokens> &tokens)
{

    if (look_ahead({TokenType::BaseTenDigit, TokenType::HexDigit, TokenType::BinaryDigit}, tokens))
    {
        auto number = match_and_remove({TokenType::BaseTenDigit,
                                        TokenType::HexDigit,
                                        TokenType::BinaryDigit},
                                       tokens);
        return std::make_shared<IntegerNode>(number.value());
    }
    else if (look_ahead(TokenType::Open_Parenthesis, tokens))
    {
        match_and_remove(TokenType::Open_Parenthesis, tokens);
        auto ret = expression(tokens);
        match_and_remove(TokenType::Close_Parenthesis, tokens);
        return ret;
    }
    return {};
}
std::optional<std::shared_ptr<ASTNode>> term(std::vector<Tokens> &tokens)
{
    auto lhs = factor(tokens);
    auto op = match_and_remove({TokenType::Multiplication, TokenType::Division}, tokens);
    while (op.has_value())
    {
        auto rhs = factor(tokens);
        lhs = std::make_shared<ExprNode>(lhs.value(), op.value(), rhs.value());
        op = match_and_remove({TokenType::Multiplication, TokenType::Division}, tokens);
    }
    return lhs;
}
std::optional<std::shared_ptr<ASTNode>> expression(std::vector<Tokens> &tokens)
{
    auto lhs = term(tokens);
    auto op = match_and_remove({TokenType::Addition, TokenType::Subtraction}, tokens);
    while (op.has_value())
    {
        auto rhs = term(tokens);
        lhs = std::make_shared<ExprNode>(lhs.value(), op.value(), rhs.value());
        op = match_and_remove({TokenType::Addition, TokenType::Subtraction}, tokens);
    }
    return lhs;
}

std::vector<std::shared_ptr<FunctionRefNode>> parse_params(std::vector<Tokens> &tokens)
{
    std::vector<std::shared_ptr<FunctionRefNode>> params;
    if (!match_and_remove(TokenType::Open_Parenthesis, tokens)
             .has_value())
    {
        return params;
    }
    while (!match_and_remove(TokenType::Close_Parenthesis, tokens).has_value())
    {
        match_and_remove(TokenType::Let, tokens);
        params.push_back(parse_function_ref(tokens)
                             .value());
        // print_tokens(tokens);
        // std::cout << "" << std::endl;
        match_and_remove(TokenType::Comma, tokens);
    }

    return params;
}
std::optional<std::shared_ptr<Type>> parse_type(std::vector<Tokens> &tokens)
{
    if (look_ahead({TokenType::Integer, TokenType::Decimal}, tokens))
    {

        auto ty = match_and_remove({TokenType::Integer, TokenType::Decimal}, tokens);
        return std::make_shared<NativeType>(ty.value());
    }
    else if (look_ahead(TokenType::Open_Parenthesis, tokens))
    {
        std::vector<std::shared_ptr<Type>> type_params;
        while (!match_and_remove(TokenType::Close_Parenthesis, tokens).has_value())
        {
            type_params.push_back(parse_type(tokens)
                                      .value());
            match_and_remove(TokenType::Comma, tokens);
        }
        match_and_remove(TokenType::Colon, tokens);
        auto retty = parse_type(tokens).value();
        return std::make_shared<FunctionType>(type_params, retty);
    }
    return {};
}
std::optional<std::shared_ptr<FunctionRefNode>> parse_function_ref(std::vector<Tokens> &tokens)
{
    auto name = match_and_remove(TokenType::Identifier, tokens);
    auto params = parse_params(tokens);
    match_and_remove(TokenType::Colon, tokens);

    auto ret = parse_type(tokens);
    return std::make_shared<FunctionRefNode>(name.value(), params, ret.value());
}
std::optional<std::shared_ptr<ASTNode>> parse_function(std::vector<Tokens> &tokens)
{
    auto func = parse_function_ref(tokens);
    std::vector<std::shared_ptr<ASTNode>> ast;

    return std::make_shared<FunctionNode>(func.value(),
                                          ast);
}
std::optional<std::shared_ptr<ASTNode>> parse_stmnts(std::vector<Tokens> &tokens)
{
    if (match_and_remove(TokenType::Let, tokens).has_value())
    {
        std::cout << "let" << std::endl;
        // print_tokens(tokens);
        return parse_function(tokens);
    }
    return {};
}
std::optional<std::shared_ptr<ASTNode>> parse_node(std::vector<Tokens> &tokens)
{
    std::cout << "parsing" << std::endl;
    return parse_stmnts(tokens);
}
