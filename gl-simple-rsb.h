#ifndef GL_SIMPLE_RSB_H
#define GL_SIMPLE_RSB_H

struct gl_simple_rsb {
    uint32_t vertex_id;
    uint32_t index_id;
    uint32_t texture_id;
    uint16_t nr_indexes;
    uint32_t shader_id;
    struct gl_simple_err* err;
};

void gl_simple_init_rsb(struct gl_simple_rsb* render);

void gl_simple_draw_rsb(struct gl_simple_rsb* render, struct gl_simple_m* matrix);

void gl_simple_load_cube_texture(struct gl_simple_rsb* render, uint8_t* buff, uint32_t w, uint32_t h);

uint32_t gl_simple_shader_rsb(struct gl_simple_err* err);

#endif
