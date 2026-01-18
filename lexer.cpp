#include "lexer.h"
#include <cctype>
#include <cstddef>

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
    WHITESPACE, COMMENT_SINGLE, COMMENT_MULTI, DONE, ERROR
};

void Lexer::set_input(const std::string& s) {
    input = s; pos = 0; line = 1; col = 1;
}

token Lexer::next_token() {
    if (pos >= input.size()) {
        return { token_type::EOF_TOKEN, "EOF", line, col };
    }

    token t{ token_type::ERROR, "", line, col };
    std::string value;
    FNC_State_Machine state = START;
    int start_line = line, start_col = col;

    while (pos < input.size() && state != DONE) {
        char ch = input[pos];
        if (ch == '\n') { ++line; col = 1; }
        else ++col;

        switch (state) {
        case START:
            if (isalpha(ch) || ch == '_') { state = IDENTIFIER; value += ch; }
            else if (isdigit(ch)) { state = NUMBER; value += ch; }
            else if (ch == '"') { state = STRING_START; }
            else if (ch == ';') { t.type = token_type::SEMICOLON; t.value = ";"; state = DONE; }
            else if (strchr("=+-*/!<>", ch)) {
                std::string op(1, ch);
                if (pos + 1 < input.size()) {
                    char next = input[pos + 1];
                    if ((ch == '=' && (next == '=' || next == '+')) ||
                        (ch == '<' && next == '=') || (ch == '>' && next == '=')) {
                        op += next; ++pos; ++col;
                    }
                }
                t.type = token_type::OPERATOR; t.value = op; state = DONE;
            }
            else if (ch == '/' && pos + 1 < input.size()) {
                if (input[pos + 1] == '/') { state = COMMENT_SINGLE; ++pos; ++col; }
                else if (input[pos + 1] == '*') { state = COMMENT_MULTI; ++pos; ++col; }
            }
            else if (isspace(ch)) { state = WHITESPACE; }
            else { std::cerr << "Skip '" << ch << "' " << line << ":" << col << std::endl; state = DONE; }
            break;

        case IDENTIFIER:
            if (isalnum(ch) || ch == '_') value += ch;
            else { --pos; --col; state = DONE; } break;
        case NUMBER:
            if (isdigit(ch) || ch == '.') value += ch;
            else { --pos; --col; state = DONE; } break;
        case STRING_START: state = STRING; value += ch; break;
        case STRING:
            value += ch;
            if (ch == '"' && value.back() != '\\') state = DONE;
            break;
        case WHITESPACE: if (!isspace(ch)) { --pos; --col; state = DONE; } break;
        case COMMENT_SINGLE: if (ch == '\n') state = DONE; break;
        case COMMENT_MULTI:
            if (ch == '*' && pos + 1 < input.size() && input[pos + 1] == '/') {
                ++pos; ++col; state = DONE;
            } break;
        }
        if (state != WHITESPACE && state != START && state != COMMENT_SINGLE && state != COMMENT_MULTI) ++pos;
    }

    if (state == IDENTIFIER || state == DONE) {
        t.type = keywords.count(value) ? token_type::KEYWORD : token_type::IDENTIFIER;
        t.value = value;
    }
    else if (state == NUMBER) {
        t.type = token_type::NUMBER; t.value = value;
    }
    else if (state == STRING) {
        t.type = token_type::STRING; t.value = value;
    }

    t.line = start_line; t.column = start_col;
    std::cout << "Token: " << token_type_to(t.type) << " [" << t.value
        << "] " << t.line << ":" << t.column << std::endl;
    return t;
}

bool Lexer::has_more() { return pos < input.size(); }

