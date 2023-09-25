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
run_cmd(const char *cmd, char *const *argv)
{
	pid_t pid;
	int dummy_status;

	if ((pid = fork()) < 0)
		die("fork: %s", strerror(errno));

	if (pid == 0) {
		execvp(cmd, argv);
		_exit(1);
	}

	waitpid(pid, &dummy_status, 0);
}

static void
utime_snapshot(const char *file, struct utimbuf *ub)
{
	struct stat sb;

	if (stat(file, &sb) < 0)
		die("stat: %s", strerror(errno));

	ub->actime  = sb.st_atime;
	ub->modtime = sb.st_mtime;
}

static void
peek(const char *file, char **cmd)
{
	struct utimbuf ub;
	utime_snapshot(file, &ub);
	run_cmd(cmd[0], cmd);
	utime(file, &ub);
}

int
main(int argc, char *argv[])
{
	if (argc > 1 && !strcmp(argv[1], "-v")) {
		puts("peek version "VERSION);
	} else if (argc < 3 || !strcmp(argv[1], "-h")) {
		puts("usage: peek [-hv] [cmd [...args] filename]");
	} else {
		peek(argv[argc - 1], argv + 1);
	}

	return 0;
}
