#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "parser/parser.h"

/* Strip quotes and backslash escapes from a raw word span. */
static char *decode_word(const char *s, size_t len)
{
	char *out = malloc(len + 1);
	size_t n = 0;

	if (!out)
		exit(1);

	for (size_t i = 0; i < len; i++) {
		char c = s[i];
		if (c == '\'') {
			for (i++; i < len && s[i] != '\''; i++)
				out[n++] = s[i];
		} else if (c == '"') {
			for (i++; i < len && s[i] != '"'; i++) {
				if (s[i] == '\\' && i + 1 < len)
					i++;
				out[n++] = s[i];
			}
		} else if (c == '\\' && i + 1 < len) {
			out[n++] = s[++i];
		} else {
			out[n++] = c;
		}
	}
	out[n] = '\0';
	return out;
}

struct alok_pipeline *parse(const char *input)
{
	struct alok_pipeline *p = calloc(1, sizeof(*p));
	struct alok_command cmd = { 0 };
	bool ok = true;

	if (!p)
		exit(1);
	init_alok_lexer(input);

	for (;;) {
		struct alok_token t = lexer();

		if (t.type == STRING) {
			command_add_argument(
				&cmd, decode_word(t.value.start, t.value.len));
		} else if (t.type == REDIRECT_IN || t.type == REDIRECT_OUT ||
			   t.type == REDIRECT_APPEND) {
			struct alok_token f = lexer();
			if (f.type != STRING) {
				ok = false;
				break;
			}
			command_add_redirect(&cmd, t.value.fd,
				     decode_word(f.value.start, f.value.len),
				     t.type == REDIRECT_APPEND);
		} else if (t.type == BACKGROUND) {
			p->background = true;
		} else if (t.type == PIPE || t.type == SEMICOLON ||
			   t.type == TOKEN_EOF) {
			if (!cmd.program) {
				/* empty input is fine; "| x" or "x |" is not */
				ok = t.type == TOKEN_EOF && p->command_count == 0;
				break;
			}
			pipeline_add_command(p, &cmd);
			memset(&cmd, 0, sizeof(cmd));
			/* only one pipeline per parse(); stop at ';' */
			if (t.type != PIPE)
				break;
		} else {
			ok = false;
			break;
		}
	}

	free_command(&cmd);
	if (!ok || p->command_count == 0) {
		free_pipeline(p);
		return NULL;
	}
	return p;
}
