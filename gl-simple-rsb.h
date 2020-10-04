#ifndef GL_SIMPLE_RSB_H
#define GL_SIMPLE_RSB_H

#include <gl-simple-base.h>

struct gl_simple_rsb {
    uint32_t vertex_id;
    uint32_t normal_id;
    uint32_t uv_id;
    uint32_t texture_id;
    uint32_t index_id;
    uint32_t nr_indexes;
    uint32_t shader_id;
    struct gl_simple_err* err;
};

void gl_simple_draw_rsb(struct gl_simple_rsb* render, struct gl_simple_m* matrix);

uint32_t gl_simple_load_cube_texture(uint8_t* buff, uint32_t w, uint32_t h);

uint32_t gl_simple_shader_rsb(struct gl_simple_err* err);

#endif
