#include "codegen.h"
#include <sstream>

std::string generate_c(const AstNode& node) {
    std::stringstream c_code;

    if (node.type == "program") {
        c_code << "#include <iostream>\n#include <string>\nusing namespace std;\n\nint main() {\n";
        for (const auto& child : node.children) {
            c_code << "    " << generate_c(child) << "\n";
        }
        c_code << "    return 0;\n}";
    }
    else if (node.type == "load" && node.value == "std.io") {
        
    }
    else if (node.type == "var_decl") {
        c_code << "int " << node.value << " = " << generate_c(node.children[0]);
    }
    else if (node.type == "print") {
        c_code << "cout << " << generate_c(node.children[0]) << " << endl";
    }
    else if (node.type == "assign") {
        c_code << node.value << " = " << generate_c(node.children[0]);
    }
    else if (node.type == "name") {
        return node.value;
    }
    else if (node.type == "number") {
        return node.value;
    }
    else if (node.type == "string") {
        return "\"" << node.value << "\"";
    }
    else if (node.type == "id") {
        return node.value;
    }

    return c_code.str();
}
