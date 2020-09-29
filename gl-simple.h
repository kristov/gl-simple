#ifndef GL_SIMPLE_H
#define GL_SIMPLE_H

#include <gl-compat.h>
#include <stdint.h>

struct gl_simple_err {
    void* data;
    void (*callback)(void* data, char* message, uint16_t length);
};

struct gl_simple_r {
    uint32_t vertex_id;
    uint32_t normal_id;
    uint32_t color_id;
    uint32_t uv_id;
    uint32_t texture_id;
    uint32_t index_id;
    uint32_t nr_indexes;
    uint32_t shader_id;
    struct gl_simple_err* err;
};

struct gl_simple_m {
    float* m;
    float* v;
    float* p;
    float* mv;
    float* mvp;
};

void gl_simple_draw_mesh_color(struct gl_simple_r* render, struct gl_simple_m* matrix);

void gl_simple_draw_mesh_texture(struct gl_simple_r* render, struct gl_simple_m* matrix);

void gl_simple_draw_skybox(struct gl_simple_r* render, struct gl_simple_m* matrix);

uint32_t gl_simple_load_integer_buffer(uint16_t* buff, uint32_t nr_ints);

uint32_t gl_simple_load_float_buffer(float* buff, uint32_t nr_floats);

uint32_t gl_simple_load_2d_texture(uint8_t* buff, uint32_t w, uint32_t h);

uint32_t gl_simple_load_cube_texture(uint8_t* buff, uint32_t w, uint32_t h);

void gl_simple_matrix_update(struct gl_simple_m* matrix);

void gl_simple_matrix_init_perspective(struct gl_simple_m* matrix, uint16_t w, uint16_t h);

uint32_t gl_simple_compile_program(const char* vert, const char* frag, struct gl_simple_err* err);

uint32_t gl_simple_color_shader(struct gl_simple_err* err);

uint32_t gl_simple_texture_shader(struct gl_simple_err* err);

void gl_simple_delete_buffer(uint32_t* gl_id);

#endif
