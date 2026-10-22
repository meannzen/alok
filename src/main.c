#include <stdio.h>

#include "shell/shell.h"

int main(void)
{
	setbuf(stdout, NULL);

	return run();
}
