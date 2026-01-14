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

static std::string token_type_to(token_type type) {
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
class state {
public:

};
enum FNC_State_Machine {
	START,
	IDENTIFIER,
	end
};
class new_state {
public:
	FNC_State_Machine current_state;
};
