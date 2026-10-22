#include <stdio.h>

#include "parser/parser.h"
#include "shell/input.h"
#include "shell/shell.h"
#include "exec/executor.h"

int run(void)
{
	struct input_buffer *input = new_input_buffer();
	struct alok_shell sh ={0};

	while (1) {
		printf("$ ");
		read_input(input);

		struct alok_pipeline *pipeline = parse(input->buffer);
		if (pipeline) {
			execute_pipeline(&sh, pipeline);
			free_pipeline(pipeline);
		}
	}

	free_input_buffer(input);
	return 0;
}
