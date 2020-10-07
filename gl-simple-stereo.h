#ifndef OPENGL_STEREO_H
#define OPENGL_STEREO_H

struct gl_simple_stereo_camera {
    float projection_matrix[16];
    float model_translation;
};

struct gl_simple_stereo;
typedef void (*gl_simple_draw_callback_t)(struct gl_simple_stereo* ostereo, void* user);

enum gl_simple_stereo_mode {
    GL_SIMPLE_MODE_STEREO = 0x01,
    GL_SIMPLE_MODE_MONO = 0x02
};

struct gl_simple_stereo {
    enum gl_simple_stereo_mode mode;
    float width;
    float height;
    float depthZ;
    float fovy;
    float aspect;
    float nearZ;
    float farZ;
    float screenZ;
    float IOD;
    float physical_width;
    float texture_shift;
    uint32_t screen_plane_vdb;
    uint32_t screen_plane_idb;
    uint32_t screen_text_offset;
    float screen_matrix[16];
    uint32_t screen_shader_program_id;
    float view_matrix[16];
    float hmd_matrix[16];
    float projection_matrix[16];
    gl_simple_draw_callback_t draw_scene_callback;
    void* user;
    void (*scene_renderer)(struct gl_simple_stereo* ostereo);
    struct gl_simple_stereo_camera left_camera;
    struct gl_simple_stereo_camera right_camera;
    struct gl_simple_stereo_camera skybox_camera;
    uint32_t screen_buffer;
    uint32_t screen_texture;
    struct gl_simple_err* err;
};

void gl_simple_stereo_draw_scene_callback(struct gl_simple_stereo* ostereo, gl_simple_draw_callback_t callback, void* user);

void gl_simple_stereo_reshape(struct gl_simple_stereo* ostereo, uint16_t width, uint16_t height);

void gl_simple_stereo_display(struct gl_simple_stereo* ostereo);

void gl_simple_stereo_init(struct gl_simple_stereo* ostereo, uint16_t width, uint16_t height, float physical_width, struct gl_simple_err* err);

#endif
