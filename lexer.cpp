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
class new_state {
public:
	FNC_State_Machine current_state;
std::string next_token(FNC_State_Machine current_state) {
		switch (current_state) {
		case START:
			if (current_state == START) {
				current_state = FNC_State_Machine::START;
				std::cout << "Finite State Machine is Started"<<"\n";

		case IDENTIFIER:
			if (current_state == END) {
				current_state = FNC_State_Machine::END;
				std::cout << "Finite State Machine is Ended"<<"\n";
			}
			}
			break;
		default:
			break;
		}
		return "UNKNOWN";
	}
};
int main() {
	new_state fsm{};
	fsm.current_state = FNC_State_Machine::START;
	fsm.next_token(fsm.current_state);
	fsm.current_state = FNC_State_Machine::END;
	fsm.next_token(fsm.current_state);
	return 0;
}
