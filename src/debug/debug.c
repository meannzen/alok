#include <stdio.h>

#include "debug.h"

void print_pipeline(const struct alok_pipeline *p)
{
	if (!p) {
		printf("pipeline: (null)\n");
		return;
	}

	printf("pipeline: %zu command(s), background=%s\n", p->command_count,
	       p->background ? "yes" : "no");

	for (size_t i = 0; i < p->command_count; i++) {
		const struct alok_command *c = &p->commands[i];

		printf("  [%zu] program: %s\n", i, c->program);
		printf("      args:");
		for (char **a = c->arguments; a && *a; a++)
			printf(" <%s>", *a);
		printf("\n");
		for (size_t r = 0; r < c->redirect_count; r++) {
			const struct alok_redirection *rd = &c->redirects[r];
			const char *op = rd->target_fd == 0 ? "<" :
					 rd->append		    ? ">>" :
								      ">";
			printf("      redirect: fd %d %s %s\n", rd->target_fd, op,
			       rd->filename);
		}
	}
}
