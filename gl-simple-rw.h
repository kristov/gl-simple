#ifndef GL_SIMPLE_RW_H
#define GL_SIMPLE_RW_H

#include <gl-simple-base.h>

struct gl_simple_rw {
    uint32_t vertex_id;
    uint32_t index_id;
    uint32_t nr_indexes;
    uint32_t shader_id;
    struct gl_simple_err* err;
};

void gl_simple_draw_rw(struct gl_simple_rw* render, struct gl_simple_m* matrix);

uint32_t gl_simple_shader_rw(struct gl_simple_err* err);

#endif
