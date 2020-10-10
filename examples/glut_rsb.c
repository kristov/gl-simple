#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <gl-simple.h>
#include <gl-matrix.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PI 3.141593f
#define TWOPI 6.283185f

float model[16];
float view[16];
float projection[16];
float mv[16];
float mvp[16];

struct gl_simple_rsb render;
struct gl_simple_m matrix;
struct gl_simple_err err;

void error_print(void* data, const char* message) {
    printf("ERROR: %s\n", message);
}

GLvoid reshape(int w, int h) {
    //
}

void motion(int x, int y) {
    float xpct = (float)x / (float)1152.0f;
    float ypct = (float)y / (float)648.0f;
    float xangle = TWOPI * xpct;
    float yangle = (PI * ypct) - (PI / 2);
    mat4_identity(&view[0]);
    mat4_rotateX(&view[0], yangle);
    mat4_rotateY(&view[0], xangle);
    gl_simple_matrix_update(&matrix);
}

GLvoid display(GLvoid) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    gl_simple_draw_rsb(&render, &matrix);
    glutSwapBuffers();
}

void do_timer(int timer_event) {
    glutPostRedisplay();
    glutTimerFunc(10, do_timer, 1);
}

void initWindowingSystem(int *argc, char **argv, int width, int height) {
    glutInit(argc, argv);
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutCreateWindow("GL Simple GLUT Demo");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(motion);
    glutTimerFunc(10, do_timer, 1);
}

void init_gl_simple(uint16_t width, uint16_t height) {
    int fd = open("rsb.bin", O_RDONLY);
    if (fd < 0) {
        return;
    }
    struct stat buf;          
    fstat(fd, &buf);
    size_t size = buf.st_size;
    uint8_t* tex = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (tex == MAP_FAILED) {
        return;
    }

    err.data = NULL;
    err.callback = error_print;
    render.err = &err;

    gl_simple_init_rsb(&render);
    render.texture_id = gl_simple_load_cube_texture(tex, 256);
    render.shader_id = gl_simple_shader_rsb(&err);

    matrix.m = &model[0];
    matrix.v = &view[0];
    matrix.p = &projection[0];
    matrix.mv = &mv[0];
    matrix.mvp = &mvp[0];

    gl_simple_matrix_init(&matrix);
    mat4_translatef(matrix.m, 0, 0, -5.0f);
    gl_simple_matrix_init_perspective(&matrix, width, height);
}

void init(int *argc, char **argv) {
    int width = 1152;
    int height = 648;
    initWindowingSystem(argc, argv, width, height);
    init_gl_simple((uint16_t)width, (uint16_t)height);
}

int32_t main(int argc, char **argv) {
    init(&argc, argv);
    glutMainLoop();
    return 0;
}
