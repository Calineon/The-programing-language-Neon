#include "codegen.h"
#include <sstream>
#include <string>


static std::string escape_string(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '\\': out += "\\\\"; break;
            case '"':  out += "\\\""; break;
            case '\n': out += "\\n"; break;
            case '\t': out += "\\t"; break;
            default:   out += c; break;
        }
    }
    return out;
}

std::string generate_c(const AstNode& node) {
    
    if (node.type == "program") {
        std::stringstream ss;
        ss << "#include <iostream>\n";
        ss << "#include <string>\n";
        ss << "using namespace std;\n\n";
        ss << "int main() {\n";

        for (const auto& child : node.children) {
            std::string line = generate_c(child);
            if (line.empty()) continue; 
            
            ss << "    " << line << "\n";
        }

        ss << "    return 0;\n";
        ss << "}\n";
        return ss.str();
    }
    else if (node.type == "load") {
        
        if (node.value == "std.io") {
            return std::string(); 
        }
        
        return std::string(); 
    }
    else if (node.type == "var_decl") {
    
        if (node.children.empty()) return std::string("// error: var_decl missing initializer");
        std::string rhs = generate_c(node.children[0]);
        
        return std::string("int ") + node.value + " = " + rhs + ";";
    }
    else if (node.type == "print") {
        if (node.children.empty()) return std::string("cout << endl;");
        std::string expr = generate_c(node.children[0]);
        
        return std::string("cout << ") + expr + " << endl;";
    }
    else if (node.type == "assign") {
        if (node.children.empty()) return std::string("// error: assign missing rhs");
        std::string rhs = generate_c(node.children[0]);
        return node.value + " = " + rhs + ";";
    }
    else if (node.type == "name") {
        
        return std::string(); 
    }
    else if (node.type == "number") {
        return node.value; 
    }
    else if (node.type == "string") {
        
        std::string s = node.value;
        
        if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
            s = s.substr(1, s.size() - 2);
        }
        return std::string("\"") + escape_string(s) + std::string("\"");
    }
    else if (node.type == "id") {
        return node.value;
    }

    
    return std::string("// unhandled node: ") + node.type;
}
