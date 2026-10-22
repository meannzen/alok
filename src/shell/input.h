#ifndef ALOK_INPUT_H
#define ALOK_INPUT_H
#include <stddef.h>
#include<stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct input_buffer {
	char *buffer;
	size_t buffer_len;
	size_t input_len;
};
struct input_buffer *new_input_buffer();
void free_input_buffer(struct input_buffer *input);
void read_input(struct input_buffer *input);
#endif
