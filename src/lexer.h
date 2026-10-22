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
	EOF
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

void init_alok_lexer(const char *input);

struct alok_token lexer();

#endif
