#include "main.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include "codegen.h"

static std::string make_output_names(const std::string& input, std::string& out_cpp) {

    std::string base = input;
    
    auto pos_slash = base.find_last_of("/\\");
    std::string filename = (pos_slash == std::string::npos) ? base : base.substr(pos_slash + 1);
    auto pos_dot = filename.find_last_of('.');
    std::string stem = (pos_dot == std::string::npos) ? filename : filename.substr(0, pos_dot);

    out_cpp = stem + ".cpp";
    return stem + "_neon_out";
}

int compile_neon(const std::string& filename) {
    try {
        std::ifstream file(filename, std::ios::binary);
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

        
        std::string out_cpp;
        std::string out_bin = make_output_names(filename, out_cpp);

        
        std::ofstream c_file(out_cpp, std::ios::binary);
        if (!c_file) {
            std::cerr << "Cannot write to " << out_cpp << std::endl;
            return 1;
        }
        c_file << c_code;
        c_file.close();

        std::cout << "\n=== GENERATED C++ (" << out_cpp << ") ===\n" << c_code << std::endl;

        
        std::string compile_cmd = "g++ -std=c++17 -O2 -o " + out_bin + " " + out_cpp;
        std::cout << "Compiling: " << compile_cmd << std::endl;
        int compile_res = std::system(compile_cmd.c_str());
        if (compile_res != 0) {
            std::cerr << "Compilation failed (exit code " << compile_res << ")." << std::endl;
            return 1;
        }

        
        std::string run_cmd = "./" + out_bin;
        std::cout << "Running: " << run_cmd << std::endl;
        int run_res = std::system(run_cmd.c_str());
        if (run_res != 0) {
            std::cerr << "Program exited with code " << run_res << std::endl;
            return 1;
        }
        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
}
