#ifndef ALOK_EXECUTOR_H
#define ALOK_EXECUTOR_H

#include "parser/ast.h"
#include "shell/shell.h"

int execute_pipeline(struct alok_shell *sh, const struct alok_pipeline *p);

#endif
