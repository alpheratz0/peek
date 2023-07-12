/*
	Copyright (C) 2022-2023 <alpheratz99@protonmail.com>

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License version 2 as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc., 59
	Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utime.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdarg.h>

static void
die(const char *fmt, ...)
{
	va_list args;

	fputs("peek: ", stderr);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputc('\n', stderr);
	exit(1);
}

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

static void
peek(const char *file, char **cmd)
{
	struct stat sb;
	struct utimbuf ub;
	pid_t pid;
	int status;

	if (stat(file, &sb) < 0)
		die("stat failed: %s", strerror(errno));

	ub.actime = sb.st_atime;
	ub.modtime = sb.st_mtime;
	pid = fork();

	if (pid < 0) {
		die("fork failed: %s", strerror(errno));
	} else if (pid == 0) {
		execvp(cmd[0], cmd);
	} else {
		waitpid(pid, &status, 0);
		utime(file, &ub);
	}
}

int
main(int argc, char **argv)
{
	if (argc < 2 || !strcmp(argv[1], "-h")) {
		usage();
	} else if (!strcmp(argv[1], "-v")) {
		version();
	} else {
		peek(argv[argc - 1], argv + 1);
	}

	return 0;
}
