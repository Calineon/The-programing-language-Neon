#include "lexer.h"
enum class token_type
{
	KEYWORD,
	IDENTIFIER,
	STRING,
	NUMBER,
	SYMONCOL
};
struct token
{
	token_type type;
	std::string value;
};
static token get_token(const std::string&value)
{
	return token{};
}
class lexer
{
public:
	bool get_next_token(const std::string& string) {
		return 0;
	}
};
