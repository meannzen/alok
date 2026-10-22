#include <stdlib.h>

#include "memory.h"
#include "parser/ast.h"

void command_add_argument(struct alok_command *cmd, char *word)
{
	size_t n = cmd->argument_count;

	/* keep room for the terminating NULL */
	cmd->arguments = GROW_ARRAY(char *, cmd->arguments, n, n + 2);
	cmd->arguments[n] = word;
	cmd->arguments[n + 1] = NULL;
	cmd->argument_count++;
	if (!cmd->program)
		cmd->program = word;
}

void command_add_redirect(struct alok_command *cmd, int fd, char *file,
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

void pipeline_add_command(struct alok_pipeline *p, struct alok_command *cmd)
{
	size_t n = p->command_count;

	p->commands = GROW_ARRAY(struct alok_command, p->commands, n, n + 1);
	p->commands[n] = *cmd;
	p->command_count++;
}

void free_command(struct alok_command *cmd)
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
