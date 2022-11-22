/*
	Copyright (C) 2022 <alpheratz99@protonmail.com>

	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License version 2 as published by the
	Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place, Suite 330, Boston, MA 02111-1307 USA

*/

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
	if (!strcmp(argv[1], "-h"))
		usage();
	if (!strcmp(argv[1], "-v"))
		version();
	if (stat(argv[argc-1], &sb) < 0)
		return 2;

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
