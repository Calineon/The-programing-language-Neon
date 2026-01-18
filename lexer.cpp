#include <iostream>
#include <cctype>
#include <string>
#include <unordered_set>
#include <cstddef>

enum class token_type {
    KEYWORD, IDENTIFIER, STRING, NUMBER, SEMICOLON, OPERATOR, ERROR, EOF_TOKEN
};

struct token {
    token_type type;
    std::string value;
    int line = 1, column = 1;
};

std::string token_type_to(token_type type) {
    switch (type) {
        case token_type::KEYWORD: return "KEYWORD";
        case token_type::IDENTIFIER: return "IDENTIFIER";
        case token_type::STRING: return "STRING";
        case token_type::NUMBER: return "NUMBER";
        case token_type::SEMICOLON: return "SEMICOLON";
        case token_type::OPERATOR: return "OPERATOR";
        case token_type::ERROR: return "ERROR";
        case token_type::EOF_TOKEN: return "EOF";
        default: return "UNKNOWN";
    }
}

enum FNC_State_Machine { 
    START, IDENTIFIER, NUMBER, STRING_START, STRING, 
    OPERATOR, WHITESPACE, COMMENT_SINGLE, COMMENT_MULTI, DONE, ERROR 
};

class Lexer {
private:
    size_t pos = 0;
    int line = 1, col = 1;
    std::string input;
    std::unordered_set<std::string> keywords{"if", "program", "int", "while", "print", "else", "return"};

public:
    FNC_State_Machine current_state = START;

    void set_input(const std::string& s) { 
        input = s; 
        pos = 0; 
        line = 1; 
        col = 1; 
    }

    token next_token() {
        // EOF handling
        if (pos >= input.size()) {
            return {token_type::EOF_TOKEN, "EOF", line, col};
        }

        token t{token_type::ERROR, "", line, col};
        std::string value;
        FNC_State_Machine state = START;
        int start_line = line, start_col = col;

        while (pos < input.size() && state != DONE) {
            char ch = input[pos];
            
            // Position tracking
            if (ch == '\n') { 
                ++line; 
                col = 1; 
            } else {
                ++col;
            }

            switch (state) {
                case START:
                    if (isalpha(ch) || ch == '_') { 
                        state = IDENTIFIER; 
                        value += ch; 
                    }
                    else if (isdigit(ch)) { 
                        state = NUMBER; 
                        value += ch; 
                    }
                    else if (ch == '"') { 
                        state = STRING_START; 
                    }
                    else if (strchr("=+-*/!<>", ch)) {
                        // Multi-char operators
                        std::string op(1, ch);
                        if (pos+1 < input.size()) {
                            char next = input[pos+1];
                            if ((ch == '=' && (next == '=' || next == '+')) ||
                                (ch == '<' && next == '=') ||
                                (ch == '>' && next == '=') ||
                                (ch == '!' && next == '=')) {
                                op += next; 
                                ++pos; 
                                ++col;
                            }
                        }
                        if (op == ";") {
                            t.type = token_type::SEMICOLON;
                        } else {
                            t.type = token_type::OPERATOR;
                        }
                        t.value = op;
                        state = DONE;
                    }
                    else if (ch == '/' && pos+1 < input.size()) {
                        if (input[pos+1] == '/') {  // // comment
                            state = COMMENT_SINGLE;
                            ++pos; ++col;
                        } else if (input[pos+1] == '*') {  // /* comment */
                            state = COMMENT_MULTI;
                            ++pos; ++col;
                        }
                    }
                    else if (isspace(ch)) { 
                        state = WHITESPACE; 
                    }
                    else {
                        std::cerr << "Skip invalid char '" << ch 
                                  << "' at " << line << ":" << col << std::endl;
                        state = DONE;
                    }
                    break;

                case IDENTIFIER:
                    if (isalnum(ch) || ch == '_') { 
                        value += ch; 
                    } else { 
                        --pos; 
                        --col; 
                        state = DONE; 
                    }
                    break;

                case NUMBER:
                    if (isdigit(ch) || ch == '.') { 
                        value += ch; 
                    } else { 
                        --pos; 
                        --col; 
                        state = DONE; 
                    }
                    break;

                case STRING_START:
                    state = STRING; 
                    value += ch; 
                    break;

                case STRING:
                    value += ch;
                    if (ch == '"' && value.back() != '\\') { 
                        state = DONE; 
                    }
                    break;

                case WHITESPACE:
                    if (!isspace(ch)) { 
                        --pos; 
                        --col; 
                        state = DONE; 
                    }
                    break;

                case COMMENT_SINGLE:
                    if (ch == '\n') state = DONE;
                    break;

                case COMMENT_MULTI:
                    if (ch == '*' && pos+1 < input.size() && input[pos+1] == '/') {
                        ++pos; ++col;
                        state = DONE;
                    }
                    break;

                default: 
                    state = ERROR; 
                    break;
            }
            
            if (state != WHITESPACE && state != START && 
                state != COMMENT_SINGLE && state != COMMENT_MULTI) {
                ++pos;
            }
        }

        // Classify token
        if (state == IDENTIFIER || state == DONE) {
            t.type = keywords.count(value) ? token_type::KEYWORD : token_type::IDENTIFIER;
            t.value = value;
        } else if (state == NUMBER) {
            t.type = token_type::NUMBER;
            t.value = value;
        } else if (state == STRING) {
            t.type = token_type::STRING;
            t.value = value;
        }

        if (t.type == token_type::ERROR) {
            t.value = "ERROR";
        }

        t.line = start_line;
        t.column = start_col;
        
        std::cout << "Token: " << token_type_to(t.type) << " [" 
                  << t.value << "] line:" << t.line 
                  << " col:" << t.column << std::endl;
        
        current_state = START;
        return t;
    }

    bool has_more() { 
        return pos < input.size(); 
    }
};

int main() {
    Lexer lexer;
    lexer.set_input(R"(
program main;
int x = 10;  // single line comment
/* multi-line
   comment */
if ("hello") { 
    x = x + 1; 
    print(x); 
}
)");

    std::cout << "=== LEXER TEST ===" << std::endl;
    while (lexer.has_more()) {
        token t = lexer.next_token();
        if (t.type == token_type::ERROR || t.type == token_type::EOF_TOKEN) {
            break;
        }
    }
    return 0;
}

