#include <utime.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

int
main(int argc, char **argv)
{
	struct stat sb;
	struct utimbuf ub;
	pid_t pid;
	int status;

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
