#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	char arg[64];
	char * path[3] = {"/sbin/rmmod", NULL, NULL};
	path[1] = arg;
	strcpy(arg, argv[1]);
	execvp(path[0], path);
	perror("Exec error \n");
	exit(1);
	return 0;
}
