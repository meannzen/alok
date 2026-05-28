#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *buffer;
  size_t buffer_len;
  size_t input_len;
} InputBuffer;

InputBuffer *new_input_buffer() {
  InputBuffer *input = (InputBuffer *)malloc(sizeof(InputBuffer));
  if (!input) {
    exit(1);
  }

  input->buffer = NULL;
  input->buffer_len = 0;
  input->input_len = 0;

  return input;
}

void free_input_buffer(InputBuffer *input) {
  free(input->buffer);
  free(input);
}

void read_input(InputBuffer *input) {
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

  InputBuffer *input = new_input_buffer();
  for (;;) {
    printf("$ ");
    read_input(input);
    if (input->input_len == 0)
      continue;

    if (strcmp(input->buffer, "exit") == 0) {
      break;
    }

    printf("%s: command not found\n", input->buffer);
  }

  free_input_buffer(input);

  return 0;
}
