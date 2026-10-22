#ifndef ALOK_AST_H
#define ALOK_AST_H

#include <stdbool.h>
#include <stddef.h>

struct alok_redirection {
	int target_fd;
	char *filename;
	bool append;
};

struct alok_command {
	char *program;
	char **arguments;
	size_t argument_count;
	struct alok_redirection *redirects;
	size_t redirect_count;
};

struct alok_pipeline {
	struct alok_command *commands;
	size_t command_count;
	bool background;
};

void command_add_argument(struct alok_command *cmd, char *word);
void command_add_redirect(struct alok_command *cmd, int fd, char *file,
			  bool append);

void pipeline_add_command(struct alok_pipeline *p, struct alok_command *cmd);

void free_command(struct alok_command *cmd);
void free_pipeline(struct alok_pipeline *pipeline);

#endif
