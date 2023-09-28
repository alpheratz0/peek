# Copyright (C) 2022-2023 <alpheratz99@protonmail.com>
# This program is free software.

VERSION = 0.1.2

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

CFLAGS = -ansi -Os -DVERSION=\"$(VERSION)\"
LDFLAGS = -s

CC = cc
