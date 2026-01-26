#include "lexer.h"
#include <cctype>
#include <cstddef>



token Lexer::next_token() {
    
    auto is_ident_start = [](char c)->bool { return std::isalpha((unsigned char)c) || c == '_'; };
    auto is_ident_part  = [](char c)->bool { return std::isalnum((unsigned char)c) || c == '_'; };
    auto is_space_ch    = [](char c)->bool { return std::isspace((unsigned char)c); };

    
    while (pos < input.size()) {
        char ch = input[pos];
        if (ch == '/' && pos + 1 < input.size() && input[pos + 1] == '/') {
        
            pos += 2;
            while (pos < input.size() && input[pos] != '\n') ++pos;
            continue;
        }
        if (ch == '/' && pos + 1 < input.size() && input[pos + 1] == '*') {
        
            pos += 2;
            while (pos + 1 < input.size()) {
                if (input[pos] == '\n') { ++line; col = 1; ++pos; continue; }
                if (input[pos] == '*' && input[pos + 1] == '/') { pos += 2; break; }
                ++pos;
            }
            continue;
        }
        if (is_space_ch(ch)) {
            
            if (ch == '\n') { ++line; col = 1; }
            else ++col;
            ++pos;
            continue;
        }
        break;
    }

    if (pos >= input.size()) {
        return { token_type::EOF_TOKEN, "", line, col };
    }

    int start_line = line;
    int start_col  = col;
    char ch = input[pos];

    
    auto advance = [&]() -> char {
        char c = input[pos++];
        if (c == '\n') { ++line; col = 1; }
        else ++col;
        return c;
    };

    
    if (is_ident_start(ch)) {
        std::string v;
        v += advance(); 
        while (pos < input.size() && is_ident_part(input[pos])) v += advance();
        token t;
        t.type = keywords.count(v) ? token_type::KEYWORD : token_type::IDENTIFIER;
        t.value = v;
        t.line = start_line; t.column = start_col;
        return t;
    }

    
    if (std::isdigit((unsigned char)ch)) {
        std::string v;
        v += advance();
        bool dot_seen = false;
        while (pos < input.size()) {
            char c = input[pos];
            if (c == '.' && !dot_seen) { dot_seen = true; v += advance(); continue; }
            if (std::isdigit((unsigned char)c)) { v += advance(); continue; }
            break;
        }
        token t{ token_type::NUMBER, v, start_line, start_col };
        return t;
    }

    if (ch == '"') {
        std::string v;
        v += advance(); 
        bool closed = false;
        while (pos < input.size()) {
            char c = advance();
            v += c;
            if (c == '\\') {
                
                if (pos < input.size()) {
                    char e = advance();
                    v += e;
                }
            } else if (c == '"') {
                closed = true;
                break;
            }
        }
        token t;
        t.type = token_type::STRING;
        t.value = v;
        t.line = start_line; t.column = start_col;
        if (!closed) {
            t.type = token_type::ERROR;
        }
        return t;
    }

    
    if (ch == ';') {
        advance();
        return token{ token_type::SEMICOLON, std::string(";"), start_line, start_col };
    }

    
    const std::string two_char_ops[] = { "==", "!=", "<=", ">=", "+=", "-=", "*=", "/=", "&&", "||" };
    const std::string one_char_ops = "=+-*/!<>";

    if (one_char_ops.find(ch) != std::string::npos) {
        std::string op;
        op += advance();
        if (pos < input.size()) {
            char next = input[pos];
            std::string maybe2 = op + next;
            bool matched = false;
            for (auto &t2 : two_char_ops) {
                if (maybe2 == t2) {
                    op += advance();
                    matched = true;
                    break;
                }
            }
            (void)matched;
        }
        return token{ token_type::OPERATOR, op, start_line, start_col };
    }

    
    std::string unknown(1, advance());
    return token{ token_type::ERROR, unknown, start_line, start_col };
}
