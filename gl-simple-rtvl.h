#ifndef GL_SIMPLE_RTVL_H
#define GL_SIMPLE_RTVL_H

struct gl_simple_rtvl {
    uint32_t vertex_id;
    uint32_t light_id;
    uint32_t uv_id;
    uint32_t texture_id;
    uint32_t index_id;
    uint32_t nr_indexes;
    uint32_t shader_id;
    struct gl_simple_err* err;
};

void gl_simple_draw_rtvl(struct gl_simple_rtvl* render, struct gl_simple_m* matrix);

uint32_t gl_simple_shader_rtvl(struct gl_simple_err* err);

#endif
