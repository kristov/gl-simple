CC := gcc
CFLAGS := -Wall -Werror

glut: CFLAGS += -DGL_SIMPLE_PLAT_GLUT
glut: EXTGL := -lGL -lGLU -lglut

rpi3: CFLAGS += -DGL_SIMPLE_PLAT_RPI3
rpi3: EXTGL := -lbcm_host -lEGL -lGLESv2
rpi3: INCD += -I/opt/vc/include
rpi3: LINKD := -L/opt/vc/lib

eglgbm: CFLAGS += -DGL_SIMPLE_PLAT_EGLGBM
eglgbm: EXTGL := -lgbm -ldrm -lEGL -lGLESv2
eglgbm: INCD += -I/usr/include/libdrm
eglgbm: EXAMPLES := eglgbm

glut: glut.c
	$(CC) $(CFLAGS) -lGL -lGLU -lglut $(shell pkg-bee --cflags --libs gl-simple gl-matrix) -o $@ $<
