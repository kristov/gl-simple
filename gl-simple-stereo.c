#include <gl-simple-base.h>
#include <gl-simple-stereo.h>
#include <gl-matrix.h>
#include <string.h>

#include <stdio.h>

#include <math.h>
#define DTR 0.0174532925

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static uint32_t gl_simple_distortion_shader(struct gl_simple_err* err) {
    const char vert[] = "#version 120\n"
        "attribute vec3 b_vertex;\n"
        "attribute vec2 b_text;\n"
        "uniform mat4 m_projection;\n"
        "varying vec2 v_texcoord;\n"
        "void main() {\n"
        "    v_texcoord = b_text;\n"
        "    gl_Position = m_projection * vec4(b_vertex, 1);\n"
        "}\n";
    const char frag[] = "#version 120\n"
        "const float PI = 3.1415926535;\n"
        "uniform float barrel_power;\n"
        "uniform sampler2D tex0;\n"
        "varying vec2 v_texcoord;\n"
        "vec2 Distort(vec2 p) {\n"
        "    float theta = atan(p.y, p.x);\n"
        "    float radius = length(p);\n"
        "    radius = pow(radius, barrel_power);\n"
        "    p.x = radius * cos(theta);\n"
        "    p.y = radius * sin(theta);\n"
        "    return 0.5 * (p + 1.0);\n"
        "}\n"
        "void main() {\n"
        "    vec2 xy = 2.0 * v_texcoord - 1.0;\n"
        "    vec2 uv;\n"
        "    vec4 color;\n"
        "    float d = length(xy);\n"
        "    if (d < 1.15) {\n"
        "        uv = Distort(xy);\n"
        "        color = texture2D(tex0, uv);\n"
        "    }\n"
        "    else {\n"
        "        color = vec4(0.0, 0.0, 0.0, 1.0);\n"
        "    }\n"
        "    gl_FragColor = color;\n"
        "}\n";
    return gl_simple_compile_program(vert, frag, err);
}

void gl_simple_stereo_store_screen_plane(struct gl_simple_stereo* ostereo) {
    float verts[] = {
        0.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f,
        2.0f, 2.0f, 0.0f
    };
    float uvs[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };
    glGenBuffers(1, &ostereo->screen_plane_vdb);
    glBindBuffer(GL_ARRAY_BUFFER, ostereo->screen_plane_vdb);
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(float), verts);
    glBufferSubData(GL_ARRAY_BUFFER, 12 * sizeof(float), 8 * sizeof(float), uvs);

    uint16_t indexes[] = {0, 1, 2, 1, 2, 3};
    glGenBuffers(1, &ostereo->screen_plane_idb);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ostereo->screen_plane_idb);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint16_t), indexes, GL_STATIC_DRAW);

    ostereo->screen_text_offset = 12 * sizeof(float); // can be just a separate buffer...
}

void gl_simple_stereo_render_screen_plane(struct gl_simple_stereo* ostereo) {
    GLuint b_vertex, b_text;

    glBindBuffer(GL_ARRAY_BUFFER, ostereo->screen_plane_vdb);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ostereo->screen_plane_idb);

    b_vertex = glGetAttribLocation(ostereo->screen_shader_program_id, "b_vertex" );
    glVertexAttribPointer(b_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_vertex);

    b_text = glGetAttribLocation(ostereo->screen_shader_program_id, "b_text" );
    glVertexAttribPointer(b_text, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(ostereo->screen_text_offset));
    glEnableVertexAttribArray(b_text);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
}

void gl_simple_stereo_create_render_texture(struct gl_simple_stereo* ostereo) {
    GLuint depthRenderBuffer;
    GLenum status;

    glGenFramebuffers(1, &ostereo->screen_buffer);
    glGenTextures(1, &ostereo->screen_texture);
    glBindFramebuffer(GL_FRAMEBUFFER, ostereo->screen_buffer);
    glBindTexture(GL_TEXTURE_2D, ostereo->screen_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ostereo->width, ostereo->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, ostereo->width, ostereo->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ostereo->screen_texture, 0);

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "FRAMEBUFFER incomplete: %d\n", (int)status);
    }

    return;
}

void gl_simple_stereo_camera_frustrum_L(struct gl_simple_stereo_camera* left_camera, double IODh, double top, double right, double frustumshift, double nearZ, double farZ) {
    left_camera->model_translation = IODh;
    mat4_frustum(
        left_camera->projection_matrix,
        -right + frustumshift,
        right + frustumshift,
        -top,
        top,
        nearZ,
        farZ
    );
}

void gl_simple_stereo_camera_frustrum_R(struct gl_simple_stereo_camera* right_camera, double IODh, double top, double right, double frustumshift, double nearZ, double farZ) {
    right_camera->model_translation = -IODh;
    mat4_frustum(
        right_camera->projection_matrix,
        -right - frustumshift,
        right - frustumshift,
        -top,
        top,
        nearZ,
        farZ
    );
}

void gl_simple_stereo_camera_frustrum_I(struct gl_simple_stereo_camera* camera, double IODh, double top, double right, double frustumshift, double nearZ, double farZ) {
    camera->model_translation = 0.0;
    mat4_frustum(
        camera->projection_matrix,
        -right,
        right,
        -top,
        top,
        nearZ,
        farZ
    );
}

void gl_simple_stereo_set_frustum(struct gl_simple_stereo* ostereo) {
    double IODh;
    double top;
    double right;
    double frustumshift;

    IODh = ostereo->IOD / 2;
    top = ostereo->nearZ * tan(DTR * ostereo->fovy / 2);
    right = ostereo->aspect * top;
    frustumshift = IODh * ostereo->nearZ / ostereo->screenZ;

    gl_simple_stereo_camera_frustrum_L(&ostereo->left_camera, IODh, top, right, frustumshift, ostereo->nearZ, ostereo->farZ);
    gl_simple_stereo_camera_frustrum_R(&ostereo->right_camera, IODh, top, right, frustumshift, ostereo->nearZ, ostereo->farZ);
    gl_simple_stereo_camera_frustrum_I(&ostereo->skybox_camera, IODh, top, right, frustumshift, ostereo->nearZ, ostereo->farZ);
}

void gl_simple_stereo_reshape(struct gl_simple_stereo* ostereo, uint16_t width, uint16_t height) {
    if (height == 0) {
        height = 1;
    }
    if (ostereo->mode == GL_SIMPLE_MODE_STEREO) {
        ostereo->width = width / 2;
    }
    else {
        ostereo->width = width;
    }
    ostereo->height = height;
    ostereo->aspect = ostereo->width / ostereo->height;
    gl_simple_stereo_set_frustum(ostereo);
}

void gl_simple_stereo_render_left_scene(struct gl_simple_stereo* ostereo) {
    GLint tex0;
    GLuint m_projection;

    glBindFramebuffer(GL_FRAMEBUFFER, ostereo->screen_buffer);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, ostereo->width, ostereo->height);

    mat4_copy(ostereo->projection_matrix, ostereo->left_camera.projection_matrix);
    mat4_identity(ostereo->view_matrix);
    mat4_multiply(ostereo->view_matrix, ostereo->hmd_matrix);
    mat4_translatef(ostereo->view_matrix, ostereo->left_camera.model_translation, 0.0, ostereo->depthZ);

    ostereo->draw_scene_callback(ostereo, ostereo->user);

    glUseProgram(ostereo->screen_shader_program_id);

    uint32_t barrel_power_id = glGetUniformLocation(ostereo->screen_shader_program_id, "barrel_power");
    glUniform1f(barrel_power_id, 1.1f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    mat4_identity(ostereo->screen_matrix);
    mat4_translatef(ostereo->screen_matrix, -1.0 + ostereo->texture_shift, -1.0, 0.0);

    m_projection = glGetUniformLocation(ostereo->screen_shader_program_id, "m_projection");
    glUniformMatrix4fv(m_projection, 1, GL_FALSE, ostereo->screen_matrix);

    glViewport(0, 0, ostereo->width, ostereo->height);

    tex0 = glGetUniformLocation(ostereo->screen_shader_program_id, "tex0");
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(tex0, 0);
    glBindTexture(GL_TEXTURE_2D, ostereo->screen_texture);

    gl_simple_stereo_render_screen_plane(ostereo);
}

void gl_simple_stereo_render_right_scene(struct gl_simple_stereo* ostereo) {
    GLint texLoc;
    GLuint m_projection;

    glBindFramebuffer(GL_FRAMEBUFFER, ostereo->screen_buffer);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, ostereo->width, ostereo->height);

    mat4_copy(ostereo->projection_matrix, ostereo->right_camera.projection_matrix);
    mat4_identity(ostereo->view_matrix);
    mat4_multiply(ostereo->view_matrix, ostereo->hmd_matrix);
    mat4_translatef(ostereo->view_matrix, ostereo->right_camera.model_translation, 0.0, ostereo->depthZ);

    ostereo->draw_scene_callback(ostereo, ostereo->user);

    glUseProgram(ostereo->screen_shader_program_id);

    uint32_t barrel_power_id = glGetUniformLocation(ostereo->screen_shader_program_id, "barrel_power");
    glUniform1f(barrel_power_id, 1.1f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    mat4_identity(ostereo->screen_matrix);
    mat4_translatef(ostereo->screen_matrix, -1.0 - ostereo->texture_shift, -1.0, 0.0);

    m_projection = glGetUniformLocation(ostereo->screen_shader_program_id, "m_projection");
    glUniformMatrix4fv(m_projection, 1, GL_FALSE, ostereo->screen_matrix);

    glViewport(ostereo->width, 0, ostereo->width, ostereo->height);

    texLoc = glGetUniformLocation(ostereo->screen_shader_program_id, "tex0");
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(texLoc, 0);
    glBindTexture(GL_TEXTURE_2D, ostereo->screen_texture);

    gl_simple_stereo_render_screen_plane(ostereo);
}

void gl_simple_stereo_render_mono_scene(struct gl_simple_stereo* ostereo) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, ostereo->width, ostereo->height);

    mat4_identity(ostereo->view_matrix);
    mat4_multiply(ostereo->view_matrix, ostereo->hmd_matrix);
    mat4_translatef(ostereo->view_matrix, ostereo->skybox_camera.model_translation, 0.0, ostereo->depthZ);

    ostereo->draw_scene_callback(ostereo, ostereo->user);
}

void gl_simple_stereo_render_stereo_scene(struct gl_simple_stereo* ostereo) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    gl_simple_stereo_render_left_scene(ostereo);
    gl_simple_stereo_render_right_scene(ostereo);
}

void gl_simple_stereo_display(struct gl_simple_stereo* ostereo) {
    if (!ostereo->draw_scene_callback) {
        fprintf(stderr, "gl_simple_stereo_ERROR: draw_scene_callback not attached\n");
        return;
    }
    ostereo->scene_renderer(ostereo);
}

void gl_simple_stereo_draw_scene_callback(struct gl_simple_stereo* ostereo, gl_simple_draw_callback_t callback, void* user) {
    if (!callback) {
        fprintf(stderr, "gl_simple_stereo_ERROR: draw_scene_callback is NULL\n");
        return;
    }
    ostereo->draw_scene_callback = callback;
    ostereo->user = user;
}

void initGL(struct gl_simple_stereo* ostereo) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP);
}

void gl_simple_stereo_camera_init(struct gl_simple_stereo_camera* camera) {
    camera->model_translation = 0.0f;
}

void gl_simple_stereo_init_system(struct gl_simple_stereo* ostereo) {
    initGL(ostereo);

    ostereo->aspect = ostereo->width / ostereo->height;
    double quart_screen_width = ostereo->physical_width / 4;
    double half_iod = ostereo->IOD / 2;

    ostereo->texture_shift = quart_screen_width - half_iod;

    //fprintf(stderr, "           Physical width (dm): %0.2f\n", ostereo->physical_width);
    //fprintf(stderr, "              Correction ratio: %0.2f\n", correction_ratio);
    //fprintf(stderr, "(half_screen_width - half_iod): %0.2f\n", (half_screen_width - half_iod));
    //fprintf(stderr, "                 Texture shift: %0.2f\n", ostereo->texture_shift);

    ostereo->screen_shader_program_id = gl_simple_distortion_shader(ostereo->err);
    gl_simple_stereo_set_frustum(ostereo);
    gl_simple_stereo_create_render_texture(ostereo);
    gl_simple_stereo_store_screen_plane(ostereo);
}

void gl_simple_stereo_load_defaults(struct gl_simple_stereo* ostereo) {
    ostereo->IOD = 0.6;
    ostereo->depthZ = 0.0;
    ostereo->fovy = 45;
    ostereo->nearZ = 0.1;
    ostereo->farZ = 300.0;
    ostereo->screenZ = 100.0;
}

void gl_simple_stereo_error(struct gl_simple_stereo* ostereo, const char* message) {
    if (ostereo->err) {
        ostereo->err->callback(ostereo->err->data, message);
    }
}

void gl_simple_stereo_mode(struct gl_simple_stereo* ostereo, enum gl_simple_stereo_mode mode, uint16_t width, uint16_t height) {
    ostereo->mode = mode;
    if (mode == GL_SIMPLE_MODE_STEREO) {
        ostereo->width = width / 2;
        ostereo->scene_renderer = gl_simple_stereo_render_stereo_scene;
        return;
    }
    else if (mode == GL_SIMPLE_MODE_MONO) {
        ostereo->width = width;
        ostereo->scene_renderer = gl_simple_stereo_render_mono_scene;
        return;
    }
    gl_simple_stereo_error(ostereo, "invalid mode (GL_STEREO_MODE_STEREO or GL_STEREO_MODE_MONO)");
    return;
}

void gl_simple_stereo_init(struct gl_simple_stereo* ostereo, uint16_t width, uint16_t height, float physical_width, struct gl_simple_err* err) {
    memset(ostereo, 0, sizeof(struct gl_simple_stereo));
    ostereo->err = err;
    ostereo->mode = GL_SIMPLE_MODE_STEREO;
    ostereo->width = width / 2;
    ostereo->scene_renderer = gl_simple_stereo_render_stereo_scene;
    ostereo->height = height;
    ostereo->physical_width = physical_width;
    gl_simple_stereo_camera_init(&ostereo->left_camera);
    gl_simple_stereo_camera_init(&ostereo->right_camera);
    gl_simple_stereo_camera_init(&ostereo->skybox_camera);
    mat4_identity(ostereo->screen_matrix);
    mat4_identity(ostereo->view_matrix);
    mat4_identity(ostereo->hmd_matrix);
    gl_simple_stereo_load_defaults(ostereo);
    gl_simple_stereo_init_system(ostereo);
}
