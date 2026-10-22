#include "debug/debug.h"
#include "lexer/lexer.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PATH_LEN 1024

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
/*
 * Return
 * -1 PATH environment variable not found" ,
 * -2 Not found Path
 *  1  succuss
 */
int executable_path(const char *program, char *path)
{
	int ret = -2;
	char *path_env = getenv("PATH");

	if (!path_env) {
		return -1;
	}

	char *path_dup = strdup(path_env);
	char *dir = strtok(path_dup, ":");
	char full_path[MAX_PATH_LEN];

	while (dir != NULL) {
		snprintf(full_path, sizeof(full_path), "%s/%s", dir, program);
		if (access(full_path, X_OK) == 0) {
			ret = 1;
			memcpy(path, full_path, MAX_PATH_LEN);
			break;
		}
		dir = strtok(NULL, ":");
	}

	free(path_dup);

	return ret;
}

void run_command(char *full_command) {
    char *args[64];
    int i = 0;
    char *token = strtok(full_command, " ");
    while (token != NULL && i < 63) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    execvp(args[0], args);

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
		struct alok_pipeline *pipeline = parse(input->buffer);
		print_pipeline(pipeline);
		free_pipeline(pipeline);
		// if (input->input_len == 0)
		// 	continue;
		// if (strcmp(input->buffer, "exit") == 0)
		// 	break;

		// if (strncmp(input->buffer, "echo ", 5) == 0) {
		// 	printf("%s\n", input->buffer + 5);
		// 	continue;
		// }

		// if (strncmp(input->buffer, "type ", 5) == 0) {
		// 	bool found_build_in = false;
		// 	for (size_t i = 0; i < build_len; i++) {
		// 		if (strcmp(input->buffer + 5, BUILT_IN[i]) ==
		// 		    0) {
		// 			found_build_in = true;
		// 			printf("%s is a shell builtin\n",
		// 			       input->buffer + 5);
		// 			break;
		// 		}
		// 	}
		// 	bool found = false;

		// 	if (!found_build_in) {
		// 		char path[MAX_PATH_LEN];
		// 		int ret = executable_path(input->buffer + 5,
		// 					  path);
		// 		if (ret == -2) {
		// 			printf("%s: not found\n",
		// 			       input->buffer + 5);
		// 		}
		// 		if (ret == -1) {
		// 			printf("PATH environment variable not found");
		// 			return 1;
		// 		}
		// 		if (ret == 1) {
		// 			printf("%s is %s\n", input->buffer + 5,
		// 			       path);
		// 		}
		// 	}
		// 	continue;
		// }
		// char path[MAX_PATH_LEN];
		// int ret = executable_path(input->buffer + 5, path);
		// if (ret == 1) {
		// 	int fd = fork();
		// 	if (fd == 0) {
  //                 printf("path %s\n", path);
		// 	} else {
		// 		wait(NULL);
		// 		printf("%s: command not found\n",
		// 		       input->buffer);
		// 	}
		// 	close(fd);
		// }
	}

	free_input_buffer(input);

	return 0;
}
