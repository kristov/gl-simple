CC := gcc
CFLAGS := -Wall -Werror -DGL_SIMPLE_PLAT_GLUT

all: gl-simple.a

debug: CFLAGS += -ggdb
debug: gl-simple.a

gl-simple.a: gl-simple.o
	ar -crs $@ $<

gl-simple.o: gl-simple.c gl-simple.h
	$(CC) $(CFLAGS) -I. $(shell pkg-bee --cflags gl-matrix) -c -o $@ $<

clean:
	rm -f gl-simple.a gl-simple.o
