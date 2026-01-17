# The-programing-language-Neon
This my language programming 
🌌 Neon Programming Language
Current Project Status: Early Development (Lexer Prototype)

Neon is a custom programming language created for learning compiler design and simple game development. Compiles to C++ or ASM with block-based syntax that eliminates curly braces.

🎯 Project Purpose
Primary Goal: Educational language for beginners that teaches lexer/parser concepts while being simpler than C++ but faster than interpreters.

Target Use Cases:

Console games (Snake without libraries)

Compiler experimentation

Learning state machines in parsing

C++/ASM code generation practice

🏗️ Development Status
text
✅ Lexer prototype (C++ token_type enum)
✅ Basic syntax design (blocks + English keywords)
🔄 Token parsing (KEYWORD/IDENTIFIER/NUMBER)
⏳ Recursive descent parser
⏳ C++ backend generator
⏳ Type system (int/double/char)
💻 Core Features (Planned)
text
Load std.io
program Name begin
    int x = 42
    print "Hello Neon!"
end
Key Advantages:

begin/end blocks → No {} confusion

Load std.io → Simple imports

Explicit types → Safer than JS, simpler than C++

English keywords → Readable for students

🎮 Example Applications
Snake Game - Console version without libraries

Calculator - Basic arithmetic parser

State Machine Simulator - FSM visualizer
