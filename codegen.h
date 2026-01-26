AstNode Parser::parse_program() {
    AstNode program{ "program", "" };


    while (current.type == token_type::KEYWORD && current.value == "Load") {
        expect(token_type::KEYWORD, "Load");
        token id = expect(token_type::IDENTIFIER);
        expect(token_type::SEMICOLON);
        program.children.emplace_back(AstNode{ "load", id.value });
    }

    
    expect(token_type::KEYWORD, "program");
    token nameTok = expect(token_type::IDENTIFIER);
    program.children.emplace_back(AstNode{ "name", nameTok.value });
    expect(token_type::SEMICOLON);
    expect(token_type::KEYWORD, "begin");

    
    while (!(current.type == token_type::KEYWORD && current.value == "end")) {
        if (current.type == token_type::EOF_TOKEN) {
            throw std::runtime_error("Unexpected end of file: expected 'end' at program end");
        }
        program.children.push_back(parse_statement());
    }
    expect(token

