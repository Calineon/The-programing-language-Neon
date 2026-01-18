#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include <string>

std::string generate_c(const AstNode& node);

#endif
