#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <string>

struct AstNode {
    std::string type;
    std::string value;
    std::vector<AstNode> children;
};

class Parser {
    Lexer& lexer;
    token current;

    void expect(token_type type, std::string value = "") {
        if (current.type != type || (value != "" && current.value != value)) {
            std::cerr << "Parse error at " << current.line << ":" << current.column
                << ": expected " << value << std::endl;
            exit(1);
        }
        current = lexer.next_token();
    }

    AstNode parse_expr();
    AstNode parse_statement();

public:
    Parser(Lexer& l) : lexer(l) { current = lexer.next_token(); }
    AstNode parse_program();
};

#endif
