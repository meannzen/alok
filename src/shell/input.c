#include "shell/input.h"
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
