#ifndef ALOK_LEXER_H
#define ALOK_LEXER_H

#include <stddef.h>

enum alok_token_type {
	STRING,
	PIPE,
	SEMICOLON,
	REDIRECT_IN,
	REDIRECT_APPEND,
	REDIRECT_OUT,
	BACKGROUND,
	ERROR,
	TOKEN_EOF
};

struct alok_token {
	enum alok_token_type type;

	union {
		int fd;
		/*
		 *  @param start:  pointer to beginning of char
		 *  @param len: the length of the array of char
		 */
		struct {
			const char *start;
			size_t len;
		};
	} value;
};

struct alok_redirection {
    int target_fd;
    char *filename;
    bool append;
};

struct alok_command {
    char *program;
    char **arguments;
    struct alok_redirection *redirects;
    size_t redirect_count;
};

struct alok_pipeline {
    struct alok_command *commands;
    size_t command_count;
    bool background;
};

void init_alok_lexer(const char *input);

struct alok_token lexer();

/* Returns NULL on a syntax error. Free with free_pipeline(). */
struct alok_pipeline *parse(const char *input);

void free_pipeline(struct alok_pipeline *pipeline);

#endif
