#ifndef GL_SIMPLE_RT_H
#define GL_SIMPLE_RT_H

struct gl_simple_rt {
    uint32_t vertex_id;
    uint32_t normal_id;
    uint32_t uv_id;
    uint32_t texture_id;
    uint32_t index_id;
    uint32_t nr_indexes;
    uint32_t shader_id;
    struct gl_simple_err* err;
};

void gl_simple_draw_rt(struct gl_simple_rt* render, struct gl_simple_m* matrix);

uint32_t gl_simple_shader_rt(struct gl_simple_err* err);

uint32_t gl_simple_load_2d_texture(uint8_t* buff, uint32_t w, uint32_t h);

#endif
