#ifndef ALOK_PARSER_H
#define ALOK_PARSER_H

#include "parser/ast.h"

/* Returns NULL on a syntax error. Free with free_pipeline(). */
struct alok_pipeline *parse(const char *input);

#endif
