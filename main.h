#ifndef NEON_MAIN_H
#define NEON_MAIN_H

#include <string>
#include <string_view>
#include "lexer.h"
#include "parser.h"

int compile_neon(const std::string& filename);


int compile_neon(std::string_view filename);

#endif // NEON_MAIN_H
