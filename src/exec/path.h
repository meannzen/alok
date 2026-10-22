#ifndef ALOK_PATH_H
#define ALOK_PATH_H

#define MAX_PATH_LEN 1024

/*
 * Search PATH for program, writing the full path into path (MAX_PATH_LEN).
 * Return
 * -1 PATH environment variable not found
 * -2 Not found Path
 *  1  success
 */
int executable_path(const char *program, char *path);

#endif
