#ifndef GL_SIMPLE_RCS_H
#define GL_SIMPLE_RCS_H

#include <gl-simple-base.h>

struct gl_simple_rcs {
    uint32_t vertex_id;
    uint32_t normal_id;
    float r;
    float g;
    float b;
    float a;
    uint32_t index_id;
    uint32_t nr_indexes;
    uint32_t shader_id;
    struct gl_simple_err* err;
};

void gl_simple_draw_rcs(struct gl_simple_rcs* render, struct gl_simple_m* matrix);

uint32_t gl_simple_shader_rcs(struct gl_simple_err* err);

#endif
