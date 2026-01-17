#include "lexer.h"
enum class token_type
{
	KEYWORD,
	IDENTIFIER,
	STRING,
	NUMBER,
	SEMICOLON
};
struct token
{
	token_type type;
	std::string value;
};

 std::string token_type_to(token_type type) {
	switch (type) {
	case token_type::KEYWORD:
		return "KEYWORD";
	case token_type::IDENTIFIER:
		return "IDENTIFIER";
	case token_type::STRING:
		return "STRING";
	case token_type::NUMBER:
		return "NUMBER";
	case token_type::SEMICOLON:
		return "SEMICOLON";
	default:
		return "UNKNOWN";
	}
}
enum FNC_State_Machine {
	START,
	IDENTIFIER,
	NUMBER,
	STRING,
	OPERATOR,
	COMMENT,
	WHITESPACE,
	DONE,
	ERROR,
	END
};
class Lexer{
public:
	FNC_State_Machine current_state=START;
	token next_token(const std::string&input){
		token t;
		t.type = token_type::IDENTIFIER;
		t.value = "";
		size_t pos = 0;
		while (pos < input.size() && current_state != DONE) {
			char ch = input[pos++];
			switch (current_state) {
			case START:
				if (isalpha(ch)) {
					current_state = IDENTIFIER;
					t.value += ch;
				}
				else if (isdigit(ch)){
					current_state = NUMBER;
					t.value += ch;
				}
				else if (ch==';') {
					t.type = token_type::SEMICOLON;
					t.value = ";";
					current_state = DONE;
				}
				else if (isspace(ch)) {
					current_state = WHITESPACE;
				}
				else {
					current_state = ERROR;
				}
				break;
			case IDENTIFIER:
				if (isalnum(ch)) {
					t.value += ch;
				}
				else {
					--pos;
					if (t.value == "if" || t.value == "program") {
						t.type = token_type::KEYWORD;
					}
					current_state = DONE;
				}
				break;
				
			case NUMBER:
				if (isdigit(ch)) {
					t.value += ch;
				}
				else {
					--pos;
					t.type = token_type::NUMBER;
					current_state = DONE;
				}
				break;

			case WHITESPACE:
				if (!isspace(ch)) {
					--pos;
					current_state = START;
				}
				break;

			default:
				current_state = ERROR;
				break;
			}
		}
		if (current_state == ERROR) {
			t.type = token_type::IDENTIFIER;
			t.value = "ERROR";
		}
		current_state = START;
		std::cout << "Token: " << token_type_to(t.type) << " [" << t.value << "]" << std::endl;
		return t;
	}
};
