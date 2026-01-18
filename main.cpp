#include "main.h"
#include <fstream>
#include <cstdlib>
#include "codegen.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: neon <file.neon>\n";
        return 1;
    }
    return compile_neon(argv[1]);
}

int compile_neon(const std::string& filename) {
   
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open " << filename << std::endl;
        return 1;
    }
    std::string neon_code((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    std::cout << "=== NEON COMPILER ===" << std::endl;
    std::cout << "Input:\n" << neon_code << std::endl;

   
    Lexer lexer;
    lexer.set_input(neon_code);

    
    Parser parser(lexer);
    AstNode ast = parser.parse_program();

    
    std::string c_code = generate_c(ast);

    
    std::ofstream c_file("output.cpp");
    c_file << c_code;
    c_file.close();

    std::cout << "\n=== GENERATED C++ ===\n" << c_code << std::endl;

    
    system("g++ -o neon_output output.cpp 2>/dev/null && ./neon_output");

    return 0;
}
