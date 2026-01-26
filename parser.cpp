#include "parser.h"
#include <stdexcept>

AstNode Parser::parse_program() {
    AstNode program{ "program", "" };

    
    while (current.type == token_type::KEYWORD && current.value == "Load") {
        expect(token_type::KEYWORD, "Load");
        token id = expect(token_type::IDENTIFIER);
        expect(token_type::SEMICOLON);
        AstNode load{ "load", id.value };
        program.children.push_back(std::move(load));
    }


    expect(token_type::KEYWORD, "program");
    token nameTok = expect(token_type::IDENTIFIER);
    program.children.push_back(AstNode{ "name", nameTok.value });
    expect(token_type::SEMICOLON);
    expect(token_type::KEYWORD, "begin");

    while (!(current.type == token_type::KEYWORD && current.value == "end")) {
        program.children.push_back(parse_statement());
    }
    expect(token_type::KEYWORD, "end");

    return program;
}

AstNode Parser::parse_statement() {
    
    if (current.type == token_type::KEYWORD && current.value == "int") {
        expect(token_type::KEYWORD, "int");
        token idTok = expect(token_type::IDENTIFIER);
        expect(token_type::OPERATOR, "=");
        AstNode expr = parse_expr();
        expect(token_type::SEMICOLON);
        AstNode decl{ "var_decl", idTok.value };
        decl.children.push_back(std::move(expr));
        return decl;
    }
    
    else if (current.type == token_type::KEYWORD && current.value == "print") {
        expect(token_type::KEYWORD, "print");
        AstNode expr = parse_expr();
        expect(token_type::SEMICOLON);
        AstNode print{ "print", "" };
        print.children.push_back(std::move(expr));
        return print;
    }
    
    else if (current.type == token_type::IDENTIFIER) {
        token idTok = expect(token_type::IDENTIFIER);
        expect(token_type::OPERATOR, "=");
        AstNode expr = parse_expr();
        expect(token_type::SEMICOLON);
        AstNode assign{ "assign", idTok.value };
        assign.children.push_back(std::move(expr));
        return assign;
    }

    throw std::runtime_error("Unknown statement at " + std::to_string(current.line) + ":" +
                             std::to_string(current.column));
}

AstNode Parser::parse_expr() {
    if (current.type == token_type::NUMBER) {
        token t = expect(token_type::NUMBER);
        AstNode num{ "number", t.value };
        return num;
    }
    else if (current.type == token_type::STRING) {
        token t = expect(token_type::STRING);
        
        std::string inner;
        if (t.value.size() >= 2 && t.value.front() == '"' && t.value.back() == '"') {
            inner = t.value.substr(1, t.value.size() - 2);
        } else {
            
            inner = t.value;
        }
        AstNode str{ "string", inner };
        return str;
    }
    else if (current.type == token_type::IDENTIFIER) {
        token t = expect(token_type::IDENTIFIER);
        AstNode id{ "id", t.value };
        return id;
    }

    throw std::runtime_error("Unknown expression at " + std::to_string(current.line) + ":" +
                             std::to_string(current.column));
}
