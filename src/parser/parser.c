#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "memory.h"

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

static void add_argument(struct alok_command *cmd, size_t *argc, char *word)
{
	/* keep room for the terminating NULL */
	cmd->arguments = GROW_ARRAY(char *, cmd->arguments, *argc, *argc + 2);
	cmd->arguments[(*argc)++] = word;
	cmd->arguments[*argc] = NULL;
	if (!cmd->program)
		cmd->program = word;
}

static void add_redirect(struct alok_command *cmd, int fd, char *file,
			 bool append)
{
	size_t n = cmd->redirect_count;
	cmd->redirects =
		GROW_ARRAY(struct alok_redirection, cmd->redirects, n, n + 1);
	cmd->redirects[n].target_fd = fd;
	cmd->redirects[n].filename = file;
	cmd->redirects[n].append = append;
	cmd->redirect_count++;
}

static void free_command(struct alok_command *cmd)
{
	for (char **a = cmd->arguments; a && *a; a++)
		free(*a);
	free(cmd->arguments);
	for (size_t i = 0; i < cmd->redirect_count; i++)
		free(cmd->redirects[i].filename);
	free(cmd->redirects);
}

void free_pipeline(struct alok_pipeline *p)
{
	if (!p)
		return;
	for (size_t i = 0; i < p->command_count; i++)
		free_command(&p->commands[i]);
	free(p->commands);
	free(p);
}

struct alok_pipeline *parse(const char *input)
{
	struct alok_pipeline *p = calloc(1, sizeof(*p));
	struct alok_command cmd = { 0 };
	size_t argc = 0;
	bool ok = true;

	if (!p)
		exit(1);
	init_alok_lexer(input);

	for (;;) {
		struct alok_token t = lexer();

		if (t.type == STRING) {
			add_argument(&cmd, &argc,
				     decode_word(t.value.start, t.value.len));
		} else if (t.type == REDIRECT_IN || t.type == REDIRECT_OUT ||
			   t.type == REDIRECT_APPEND) {
			struct alok_token f = lexer();
			if (f.type != STRING) {
				ok = false;
				break;
			}
			add_redirect(&cmd, t.value.fd,
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
			p->commands = GROW_ARRAY(struct alok_command,
						 p->commands, p->command_count,
						 p->command_count + 1);
			p->commands[p->command_count++] = cmd;
			memset(&cmd, 0, sizeof(cmd));
			argc = 0;
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
