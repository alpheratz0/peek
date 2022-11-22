#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utime.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

static void
usage(void)
{
	puts("usage: peek [-hv] [cmd [...args] filename]");
	exit(0);
}

static void
version(void)
{
	puts("peek version "VERSION);
	exit(0);
}

int
main(int argc, char **argv)
{
	struct stat sb;
	struct utimbuf ub;
	pid_t pid;
	int status;

	if (argc < 2)
		return 1;

	if (!strcmp(argv[1], "-h")) usage();
	if (!strcmp(argv[1], "-v")) version();

	stat(argv[argc-1], &sb);
	ub.actime = sb.st_atime;
	ub.modtime = sb.st_mtime;
	pid = fork();

	if (pid == 0) {
		execvp(argv[1], &argv[1]);
	} else {
		waitpid(pid, &status, 0);
		utime(argv[argc-1], &ub);
	}

	return 0;
}
