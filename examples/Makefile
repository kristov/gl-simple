CC := gcc
CFLAGS := -Wall -Werror -DGL_SIMPLE_PLAT_GLUT
LINK := -lGL -lGLU -lglut

GLUT :=
GLUT += glut_rc
GLUT += glut_rcs

#glut: CFLAGS += -DGL_SIMPLE_PLAT_GLUT
#glut: LINK := -lGL -lGLU -lglut

#rpi3: CFLAGS += -DGL_SIMPLE_PLAT_RPI3
#rpi3: INC += -I/opt/vc/include
#rpi3: LINK := -L/opt/vc/lib -lbcm_host -lEGL -lGLESv2

#eglgbm: CFLAGS += -DGL_SIMPLE_PLAT_EGLGBM
#eglgbm: INC += -I/usr/include/libdrm
#eglgbm: LINK := -lgbm -ldrm -lEGL -lGLESv2

all: $(GLUT)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(shell pkg-bee --cflags --libs gl-simple gl-matrix) $(LINK)

clean:
	rm -f $(GLUT)
