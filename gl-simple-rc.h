#ifndef GL_SIMPLE_RC_H
#define GL_SIMPLE_RC_H

#include <gl-simple-base.h>

struct gl_simple_rc {
    uint32_t vertex_id;
    uint32_t normal_id;
    uint32_t color_id;
    uint32_t index_id;
    uint32_t nr_indexes;
    uint32_t shader_id;
    struct gl_simple_err* err;
};

void gl_simple_draw_rc(struct gl_simple_rc* render, struct gl_simple_m* matrix);

uint32_t gl_simple_rc_shader(struct gl_simple_err* err);

#endif
