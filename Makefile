CC := gcc
CFLAGS := -Wall -Werror -DGL_SIMPLE_PLAT_GLUT

OBJECTS :=
OBJECTS += gl-simple-base.o
OBJECTS += gl-simple-rcs.o
OBJECTS += gl-simple-rcsvl.o
OBJECTS += gl-simple-rc.o
OBJECTS += gl-simple-rt.o
OBJECTS += gl-simple-rtvl.o
OBJECTS += gl-simple-rsb.o
OBJECTS += gl-simple-rw.o
OBJECTS += gl-simple-stereo.o

HEADERS := $(OBJECTS:.o=.h)

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
