#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct input_buffer {
  char *buffer;
  size_t buffer_len;
  size_t input_len;
};

struct input_buffer *new_input_buffer() {
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

void free_input_buffer(struct input_buffer *input) {
  free(input->buffer);
  free(input);
}

void read_input(struct input_buffer *input) {
  ssize_t byte_read = getline(&(input->buffer), &(input->buffer_len), stdin);

  if (byte_read == -1) {
    free_input_buffer(input);
    exit(1);
  }

  input->input_len = byte_read - 1;
  input->buffer[byte_read - 1] = '\0';
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  struct input_buffer *input = new_input_buffer();

  while (1) {
    printf("$ ");
    read_input(input);
    if (input->input_len == 0)
      continue;

    if (strcmp(input->buffer, "exit") == 0)
      break;

    if(strncmp(input->buffer, "echo ", 5)== 0) {
	  printf("%s\n", input->buffer + 5);
	  continue;
    }

    printf("%s: command not found\n", input->buffer);
  }

  free_input_buffer(input);

  return 0;
}
