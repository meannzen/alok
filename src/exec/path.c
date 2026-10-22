#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exec/path.h"

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
