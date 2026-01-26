#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

struct AstNode {
    std::string type;
    std::string value;
    std::vector<AstNode> children;
};

class Parser {
    Lexer& lexer;
    token current;


    void advance() { current = lexer.next_token(); }

    
    void expect(token_type type, const std::string& value = "") {
        if (current.type != type || (!value.empty() && current.value != value)) {
            std::string msg = "Parse error at " + std::to_string(current.line) + ":" +
                              std::to_string(current.column) + ": expected ";
            if (!value.empty()) msg += "'" + value + "'";
            else msg += token_type_to(type);
            msg += ", got '" + current.value + "'";
            throw std::runtime_error(msg);
        }
        advance();
    }

    
    const token& peek() const { return current; }

    
    bool at_end() const { return current.type == token_type::EOF_TOKEN; }

    
    AstNode parse_expr();
    AstNode parse_statement();

public:
    
    explicit Parser(Lexer& l) : lexer(l) {
        advance(); 
    }

    AstNode parse_program();
};

#endif // PARSER_H
