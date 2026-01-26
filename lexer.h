#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_set>
#include <cstddef>

enum class token_type {
    KEYWORD,
    IDENTIFIER,
    STRING,
    NUMBER,
    SEMICOLON,
    OPERATOR,
    ERROR,
    EOF_TOKEN
};

struct token {
    token_type type;
    std::string value;
    int line = 1;
    int column = 1;
};

inline std::string token_type_to(token_type type) {
    switch (type) {
        case token_type::KEYWORD:     return "KEYWORD";
        case token_type::IDENTIFIER:  return "IDENTIFIER";
        case token_type::STRING:      return "STRING";
        case token_type::NUMBER:      return "NUMBER";
        case token_type::SEMICOLON:   return "SEMICOLON";
        case token_type::OPERATOR:    return "OPERATOR";
        case token_type::ERROR:       return "ERROR";
        case token_type::EOF_TOKEN:   return "EOF";
    }
    return "UNKNOWN";
}

class Lexer {
private:
    std::size_t pos = 0;
    int line = 1;
    int column = 1;
    std::string input;

    
    inline static const std::unordered_set<std::string> keywords{
        "if", "program", "int", "while", "print", "else", "return",
        "Load", "begin", "end"
    };

public:
    Lexer() = default;
    explicit Lexer(std::string s) : pos(0), line(1), column(1), input(std::move(s)) {}

    
    token next_token();

    
    bool has_more() const { return pos < input.size(); }

    
    void set_input(const std::string& s) {
        input = s;
        pos = 0;
        line = 1;
        column = 1;
    }
    void set_input(std::string&& s) {
        input = std::move(s);
        pos = 0;
        line = 1;
        column = 1;
    }
};

#endif // LEXER_H
