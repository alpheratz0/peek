# Copyright (C) 2022-2023 <alpheratz99@protonmail.com>
# This program is free software.

VERSION   = 0.1.0

CC        = cc
CFLAGS    = -ansi -pedantic -Wall -Wextra -Os -DVERSION=\"$(VERSION)\"
LDFLAGS   = -s

PREFIX    = /usr/local
MANPREFIX = $(PREFIX)/share/man
