#ifndef GL_SIMPLE_RCSVL_H
#define GL_SIMPLE_RCSVL_H

struct gl_simple_rcsvl {
    uint32_t vertex_id;
    uint32_t light_id;
    float r;
    float g;
    float b;
    float a;
    uint32_t index_id;
    uint32_t nr_indexes;
    uint32_t shader_id;
    struct gl_simple_err* err;
};

void gl_simple_draw_rcsvl(struct gl_simple_rcsvl* render, struct gl_simple_m* matrix);

uint32_t gl_simple_shader_rcsvl(struct gl_simple_err* err);

#endif
