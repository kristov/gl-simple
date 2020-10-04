#ifndef GL_SIMPLE_BASE_H
#define GL_SIMPLE_BASE_H

#include <gl-compat.h>
#include <stdint.h>

struct gl_simple_err {
    void* data;
    void (*callback)(void* data, char* message, uint16_t length);
};

void gl_simple_print_error_int(char *file, int line, struct gl_simple_err* err);

#define gl_simple_print_error(err) gl_simple_print_error_int(__FILE__, __LINE__, err)

struct gl_simple_m {
    float* m;
    float* v;
    float* p;
    float* mv;
    float* mvp;
};

uint32_t gl_simple_load_integer_buffer(uint16_t* buff, uint32_t nr_ints);

uint32_t gl_simple_load_float_buffer(float* buff, uint32_t nr_floats);

void gl_simple_matrix_init(struct gl_simple_m* matrix);

void gl_simple_matrix_update(struct gl_simple_m* matrix);

void gl_simple_matrix_init_perspective(struct gl_simple_m* matrix, uint16_t w, uint16_t h);

uint32_t gl_simple_compile_program(const char* vert, const char* frag, struct gl_simple_err* err);

void gl_simple_delete_buffer(uint32_t* gl_id);

#endif
