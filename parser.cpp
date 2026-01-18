#include "parser.h"

AstNode Parser::parse_program() {
    AstNode program{ "program", "" };

    
    while (current.value == "Load") {
        expect(token_type::KEYWORD);
        AstNode load{ "load", expect(token_type::IDENTIFIER).value };
        expect(token_type::SEMICOLON);
        program.children.push_back(load);
    }

    expect(token_type::KEYWORD, "program");
    program.children.push_back({ "name", expect(token_type::IDENTIFIER).value });
    expect(token_type::SEMICOLON);
    expect(token_type::KEYWORD, "begin");

    while (current.type != token_type::KEYWORD || current.value != "end") {
        program.children.push_back(parse_statement());
    }
    expect(token_type::KEYWORD, "end");

    return program;
}

AstNode Parser::parse_statement() {
    if (current.value == "int") {
        expect(token_type::KEYWORD);
        std::string name = expect(token_type::IDENTIFIER).value;
        expect(token_type::OPERATOR, "=");
        AstNode expr = parse_expr();
        expect(token_type::SEMICOLON);
        AstNode decl{ "var_decl", name };
        decl.children.push_back(expr);
        return decl;
    }
    else if (current.value == "print") {
        expect(token_type::KEYWORD);
        AstNode expr = parse_expr();
        expect(token_type::SEMICOLON);
        AstNode print{ "print", "" };
        print.children.push_back(expr);
        return print;
    }
    else if (current.type == token_type::IDENTIFIER) {
        std::string name = current.value;
        expect(token_type::IDENTIFIER);
        expect(token_type::OPERATOR, "=");
        AstNode expr = parse_expr();
        expect(token_type::SEMICOLON);
        AstNode assign{ "assign", name };
        assign.children.push_back(expr);
        return assign;
    }
    std::cerr << "Unknown statement\n"; exit(1);
    return {};
}

AstNode Parser::parse_expr() {
    if (current.type == token_type::NUMBER) {
        AstNode num{ "number", current.value };
        expect(token_type::NUMBER);
        return num;
    }
    else if (current.type == token_type::STRING) {
        AstNode str{ "string", current.value.substr(1, current.value.size() - 2) };
        expect(token_type::STRING);
        return str;
    }
    else if (current.type == token_type::IDENTIFIER) {
        AstNode id{ "id", current.value };
        expect(token_type::IDENTIFIER);
        return id;
    }
    std::cerr << "Unknown expression\n"; exit(1);
    return {};
}
