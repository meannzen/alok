#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct input_buffer {
	char *buffer;
	size_t buffer_len;
	size_t input_len;
};

struct input_buffer *new_input_buffer()
{
	struct input_buffer *input =
		(struct input_buffer *)malloc(sizeof(struct input_buffer));

	if (!input) {
		exit(1);
	}

	input->buffer = NULL;
	input->buffer_len = 0;
	input->input_len = 0;

	return input;
}

void free_input_buffer(struct input_buffer *input)
{
	free(input->buffer);
	free(input);
}

void read_input(struct input_buffer *input)
{
	ssize_t byte_read =
		getline(&(input->buffer), &(input->buffer_len), stdin);

	if (byte_read == -1) {
		free_input_buffer(input);
		exit(1);
	}

	input->input_len = byte_read - 1;
	input->buffer[byte_read - 1] = '\0';
}

int main(int argc, char *argv[])
{
	setbuf(stdout, NULL);
	struct input_buffer *input = new_input_buffer();

	const char *BUILT_IN[] = { "exit", "type", "echo" };

	const size_t build_len = sizeof(BUILT_IN) / sizeof(size_t);

	while (1) {
		printf("$ ");
		read_input(input);
		if (input->input_len == 0)
			continue;

		if (strcmp(input->buffer, "exit") == 0)
			break;

		if (strncmp(input->buffer, "echo ", 5) == 0) {
			printf("%s\n", input->buffer + 5);
			continue;
		}

		if (strncmp(input->buffer, "type ", 5) == 0) {
			bool found_build_in = false;
			for (size_t i = 0; i < build_len; i++) {
				if (strcmp(input->buffer + 5, BUILT_IN[i]) ==
				    0) {
					found_build_in = true;
					printf("%s is a shell builtin\n",
					       input->buffer + 5);
					break;
				}
			}
			if (!found_build_in) {
				printf("%s: not found\n", input->buffer + 5);
			}
			continue;
		}

		printf("%s: command not found\n", input->buffer);
	}

	free_input_buffer(input);

	return 0;
}
