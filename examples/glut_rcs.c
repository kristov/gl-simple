#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <gl-simple.h>
#include <gl-matrix.h>

#define PI    3.141593f
#define TWOPI 6.283185f

float model[16];
float view[16];
float projection[16];
float mv[16];
float mvp[16];

struct gl_simple_rcs render;
struct gl_simple_m matrix;
struct gl_simple_err err;

float verts[] = {
    -1.0f, -1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,
    -1.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  0.0f
};

float norms[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f
};

uint16_t indexes[] = {0, 1, 2, 1, 2, 3};

void error_print(void* data, char* message, uint16_t len) {
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
    mat4_identity(&model[0]);
    mat4_translatef(&model[0], 0, 0, -5.0f);
    mat4_rotateX(&model[0], yangle);
    mat4_rotateY(&model[0], xangle);
    gl_simple_matrix_update(&matrix);
}

GLvoid display(GLvoid) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    gl_simple_draw_rcs(&render, &matrix);
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
    render.vertex_id = gl_simple_load_float_buffer(verts, 12);
    render.normal_id = gl_simple_load_float_buffer(norms, 12);
    render.index_id = gl_simple_load_integer_buffer(indexes, 6);
    render.nr_indexes = 6;
    render.r = 0.0f;
    render.g = 0.7f;
    render.b = 0.7f;
    render.a = 1.0f;

    err.data = NULL;
    err.callback = error_print;
    render.shader_id = gl_simple_rcs_shader(&err);

    render.err = &err;

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
