CC := gcc
CFLAGS := -Wall -Werror -ggdb -DGL_SIMPLE_PLAT_GLUT
INCD := -I. -I../gl-matrix

all: gl-simple.a

debug: CFLAGS += -ggdb
debug: gl-simple.a

bin/glut: CFLAGS += -DGL_SIMPLE_PLAT_GLUT
bin/glut: EXTGL := -lGL -lGLU -lglut
bin/glut: gl-simple.o

bin/rpi3: CFLAGS += -DGL_SIMPLE_PLAT_RPI3
bin/rpi3: EXTGL := -lbcm_host -lEGL -lGLESv2
bin/rpi3: INCD += -I/opt/vc/include
bin/rpi3: LINKD := -L/opt/vc/lib
bin/rpi3: gl-simple.o

bin/eglgbm: CFLAGS += -DGL_SIMPLE_PLAT_EGLGBM
bin/eglgbm: EXTGL := -lgbm -ldrm -lEGL -lGLESv2
bin/eglgbm: INCD += -I/usr/include/libdrm
bin/eglgbm: EXAMPLES := eglgbm
bin/eglgbm: gl-simple.o

gl-simple.a: gl-simple.o
	ar -crs $@ $<

gl-simple.o: gl-simple.c gl-simple.h
	$(CC) $(CFLAGS) $(EXTGL) $(INCD) $(LINKD) $(DEFS) -c -o $@ $<

bin/glut: examples/glut.c gl-simple.o
	$(CC) $(CFLAGS) $(EXTGL) -o $@ $< gl-simple.o

clean:
	rm -f gl-matrix.a gl-matrix.o
