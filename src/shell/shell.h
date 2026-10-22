#ifndef ALOK_SHELL_H
#define ALOK_SHELL_H

struct alok_shell {
	char *cwd;
	char **env;
};

/* REPL: prompt, read, parse, (execute). Returns when input ends. */
int run(void);

#endif
