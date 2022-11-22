.POSIX:
.PHONY: all clean install uninstall dist

include config.mk

all: peek

peek: peek.o
	$(CC) $(LDFLAGS) -o peek peek.o

clean:
	rm -f peek peek.o peek-$(VERSION).tar.gz

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f peek $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/peek

dist: clean
	mkdir -p peek-$(VERSION)
	cp -R COPYING config.mk Makefile README peek.c peek-$(VERSION)
	tar -cf peek-$(VERSION).tar peek-$(VERSION)
	gzip peek-$(VERSION).tar
	rm -rf peek-$(VERSION)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/peek
