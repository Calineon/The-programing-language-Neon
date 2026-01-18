#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_set>
#include <iostream>

enum class token_type {
    KEYWORD, IDENTIFIER, STRING, NUMBER, SEMICOLON, OPERATOR, ERROR, EOF_TOKEN
};

struct token {
    token_type type;
    std::string value;
    int line = 1, column = 1;
};

std::string token_type_to(token_type type);

class Lexer {
private:
    size_t pos = 0;
    int line = 1, col = 1;
    std::string input;
    std::unordered_set<std::string> keywords{
        "if", "program", "int", "while", "print", "else", "return",
        "Load", "begin", "end"  
    };

public:
    token next_token();
    bool has_more();
    void set_input(const std::string& s);
};

#endif
