CC := gcc
CFLAGS := -Wall -Werror -DGL_SIMPLE_PLAT_GLUT

OBJECTS :=
OBJECTS += gl-simple-base.o
OBJECTS += gl-simple-rcs.o
OBJECTS += gl-simple-rc.o
OBJECTS += gl-simple-rt.o
OBJECTS += gl-simple-rsb.o

HEADERS :=
HEADERS += gl-simple-base.h
HEADERS += gl-simple-rcs.h
HEADERS += gl-simple-rc.h
HEADERS += gl-simple-rt.h
HEADERS += gl-simple-rsb.h

all: gl-simple.a gl-simple.h

debug: CFLAGS += -ggdb
debug: gl-simple.a gl-simple.h

gl-simple.a: $(OBJECTS)
	ar -crs $@ $(OBJECTS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -I. $(shell pkg-bee --cflags gl-matrix) -c -o $@ $<

gl-simple.h: $(HEADERS)
	cat $(HEADERS) > gl-simple.h

clean:
	rm -f gl-simple.a $(OBJECTS)
