#include <gl-simple.h>
#include <gl-matrix.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

void print_gl_error_int(char *file, int line, struct gl_simple_err* err) {
    if (!err) {
        printf("no err\n");
        return;
    }
    GLenum glErr = glGetError();
    if (!glErr) {
        return;
    }
    printf("glErr\n");
    char* log = malloc(sizeof(char) * 256);
    if (!log) {
        err->callback(err->data, "LOG MALLOC ERROR", 17);
    }
    log[225] = 0;
    switch (glErr) {
        case GL_INVALID_ENUM:
            snprintf(log, 255, "GL_INVALID_ENUM in file=%s line=%d code=%d", file, line, glErr);
            break;
        case GL_INVALID_VALUE:
            snprintf(log, 255, "GL_INVALID_VALUE in file=%s line=%d code=%d", file, line, glErr);
            break;
        case GL_INVALID_OPERATION:
            snprintf(log, 255, "GL_INVALID_OPERATION in file=%s line=%d code=%d", file, line, glErr);
            break;
        case GL_STACK_OVERFLOW:
            snprintf(log, 255, "GL_STACK_OVERFLOW in file=%s line=%d code=%d", file, line, glErr);
            break;
        case GL_STACK_UNDERFLOW:
            snprintf(log, 255, "GL_STACK_UNDERFLOW in file=%s line=%d code=%d", file, line, glErr);
            break;
        case GL_OUT_OF_MEMORY:
            snprintf(log, 255, "GL_OUT_OF_MEMORY in file=%s line=%d code=%d", file, line, glErr);
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            snprintf(log, 255, "GL_INVALID_FRAMEBUFFER_OPERATION in file=%s line=%d code=%d", file, line, glErr);
            break;
        default:
            snprintf(log, 255, "[UNKNOWN ERROR] in file=%s line=%d code=%d", file, line, glErr);
            break;
    }
    err->callback(err->data, log, strlen(log));
    free(log);
}

#define print_gl_error(err) print_gl_error_int(__FILE__, __LINE__, err)

void gl_simple_draw_mesh_color(struct gl_simple_r* render, struct gl_simple_m* matrix) {
    GLuint shader_id = (GLuint)render->shader_id;
    glUseProgram(shader_id);
    print_gl_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->vertex_id);
    GLuint b_vertex = glGetAttribLocation(shader_id, "b_vertex");
    glVertexAttribPointer(b_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_vertex);
    print_gl_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->normal_id);
    GLuint b_normal = glGetAttribLocation(shader_id, "b_normal");
    glVertexAttribPointer(b_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_normal);
    print_gl_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->color_id);
    GLuint b_color = glGetAttribLocation(shader_id, "b_color");
    glVertexAttribPointer(b_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_color);
    print_gl_error(render->err);

    GLuint m_mvp = glGetUniformLocation(shader_id, "m_mvp");
    glUniformMatrix4fv(m_mvp, 1, GL_FALSE, matrix->mvp);
    print_gl_error(render->err);

    GLuint m_mv = glGetUniformLocation(shader_id, "m_mv");
    glUniformMatrix4fv(m_mv, 1, GL_FALSE, matrix->mv);
    print_gl_error(render->err);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)render->index_id);
    glDrawElements(GL_TRIANGLES, (GLuint)render->nr_indexes, GL_UNSIGNED_SHORT, NULL);
    print_gl_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    print_gl_error(render->err);
}

void gl_simple_draw_mesh_texture(struct gl_simple_r* render, struct gl_simple_m* matrix) {
    GLuint shader_id = (GLuint)render->shader_id;
    glUseProgram(shader_id);
    print_gl_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->vertex_id);
    GLuint b_vertex = glGetAttribLocation(shader_id, "b_vertex");
    glVertexAttribPointer(b_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_vertex);
    print_gl_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->normal_id);
    GLuint b_normal = glGetAttribLocation(shader_id, "b_normal");
    glVertexAttribPointer(b_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_normal);
    print_gl_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->uv_id);
    GLuint b_uv = glGetAttribLocation(shader_id, "b_uv");
    glVertexAttribPointer(b_uv, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_uv);
    print_gl_error(render->err);

    GLuint s_tex = glGetUniformLocation(shader_id, "s_tex");
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(s_tex, 1);
    glBindTexture(GL_TEXTURE_2D, (GLuint)render->texture_id);
    print_gl_error(render->err);

    GLuint m_mvp = glGetUniformLocation(shader_id, "m_mvp");
    glUniformMatrix4fv(m_mvp, 1, GL_FALSE, matrix->mvp);
    print_gl_error(render->err);

    GLuint m_mv = glGetUniformLocation(shader_id, "m_mv");
    glUniformMatrix4fv(m_mv, 1, GL_FALSE, matrix->mv);
    print_gl_error(render->err);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)render->index_id);
    glDrawElements(GL_TRIANGLES, (GLuint)render->nr_indexes, GL_UNSIGNED_SHORT, NULL);
    print_gl_error(render->err);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    print_gl_error(render->err);
}

void gl_simple_draw_skybox(struct gl_simple_r* render, struct gl_simple_m* matrix) {
    GLuint shader_id = (GLuint)render->shader_id;
    glUseProgram(shader_id);
    glDisable(GL_DEPTH_TEST);
    print_gl_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->vertex_id);
    GLuint b_vertex = glGetAttribLocation(shader_id, "b_vertex");
    glVertexAttribPointer(b_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_vertex);
    print_gl_error(render->err);

    GLuint s_tex = glGetUniformLocation(shader_id, "s_tex");
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(s_tex, 1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)render->texture_id);
    print_gl_error(render->err);

    GLuint m_mvp = glGetUniformLocation(shader_id, "m_mvp");
    glUniformMatrix4fv(m_mvp, 1, GL_FALSE, matrix->mvp);
    print_gl_error(render->err);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)render->index_id);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
    print_gl_error(render->err);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    print_gl_error(render->err);
}

uint32_t gl_simple_load_integer_buffer(uint16_t* buff, uint32_t nr_ints) {
    uint32_t destination;
    glGenBuffers(1, &destination);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, destination);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nr_ints * sizeof(uint16_t), (void*)buff, GL_STATIC_DRAW);
    return destination;
}

uint32_t gl_simple_load_float_buffer(float* buff, uint32_t nr_floats) {
    uint32_t destination;
    glGenBuffers(1, &destination);
    glBindBuffer(GL_ARRAY_BUFFER, destination);
    glBufferData(GL_ARRAY_BUFFER, nr_floats * sizeof(float), (void*)buff, GL_STATIC_DRAW);
    return destination;
}

uint32_t gl_simple_load_2d_texture(uint8_t* buff, uint32_t w, uint32_t h) {
    uint32_t destination;
    glGenTextures(1, &destination);
    glBindTexture(GL_TEXTURE_2D, destination);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)buff);
    return destination;
}

uint32_t gl_simple_load_cube_texture(uint8_t* buff, uint32_t w, uint32_t h) {
    uint32_t destination;
    uint32_t o = 0;
    uint32_t po = w * h * 4;
    glGenTextures(1, &destination);
    glBindTexture(GL_TEXTURE_CUBE_MAP, destination);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, w, h, 0, GL_RGBA8, GL_UNSIGNED_BYTE, &buff[o]);
    o += po;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, w, h, 0, GL_RGBA8, GL_UNSIGNED_BYTE, &buff[o]);
    o += po;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, w, h, 0, GL_RGBA8, GL_UNSIGNED_BYTE, &buff[o]);
    o += po;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, w, h, 0, GL_RGBA8, GL_UNSIGNED_BYTE, &buff[o]);
    o += po;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, w, h, 0, GL_RGBA8, GL_UNSIGNED_BYTE, &buff[o]);
    o += po;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, w, h, 0, GL_RGBA8, GL_UNSIGNED_BYTE, &buff[o]);
    return destination;
}

void gl_simple_matrix_init(struct gl_simple_m* matrix) {
    mat4_identity(matrix->m);
    mat4_identity(matrix->v);
    mat4_identity(matrix->p);
    mat4_identity(matrix->mv);
    mat4_identity(matrix->mvp);
}

void gl_simple_matrix_update(struct gl_simple_m* matrix) {
    mat4_copy(matrix->mv, matrix->v);
    mat4_multiply(matrix->mv, matrix->m);
    mat4_copy(matrix->mvp, matrix->p);
    mat4_multiply(matrix->mvp, matrix->v);
    mat4_multiply(matrix->mvp, matrix->m);
}

void gl_simple_matrix_init_perspective(struct gl_simple_m* matrix, uint16_t w, uint16_t h) {
    float fovy = 45.0f;
    float near_z = 0.1f;
    float far_z = 300.0f;
    float top = near_z * tan(0.0174532925 * fovy / 2);
    float right = (w / h) * top;
    mat4_frustum(matrix->p, -right, right, -top, top, near_z, far_z);
    gl_simple_matrix_update(matrix);
}

static void shader_err(GLuint obj, struct gl_simple_err* err) {
    int32_t length;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, (GLint*)&length);
    char* log = malloc(length);
    if (!log) {
        err->callback(err->data, "LOG MALLOC ERROR", 17);
    }
    glGetShaderInfoLog(obj, length, NULL, log);
    err->callback(err->data, log, length);
    free(log);
}

static void program_err(GLuint obj, struct gl_simple_err* err) {
    int32_t length;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, (GLint*)&length);
    char* log = malloc(length);
    if (!log) {
        err->callback(err->data, "LOG MALLOC ERROR", 17);
    }
    glGetProgramInfoLog(obj, length, NULL, log);
    err->callback(err->data, log, length);
    free(log);
}

GLuint gl_simple_compile_shader(GLenum type, const char* src, struct gl_simple_err* err) {
    GLint length = strlen(src);
    GLint ok = 0;
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&src, &length);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        if (err) {
            shader_err(shader, err);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

uint32_t gl_simple_compile_program(const char* vert, const char* frag, struct gl_simple_err* err) {
    GLint ok = 0;
    GLuint vert_shader = gl_simple_compile_shader(GL_VERTEX_SHADER, vert, err);
    GLuint frag_shader = gl_simple_compile_shader(GL_FRAGMENT_SHADER, frag, err);
    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        if (err) {
            program_err(program, err);
        }
        glDeleteProgram(program);
        return 0;
    }
    return (uint32_t)program;
}

uint32_t gl_simple_color_shader(struct gl_simple_err* err) {
#ifdef GL_SIMPLE_PLAT_RPI3
    const char* vert = "#version 100\n"
        "precision mediump int;\n"
        "precision mediump float;\n"
#else
    const char* vert = "#version 120\n"
#endif
        "attribute vec3 b_vertex;\n"
        "attribute vec3 b_normal;\n"
        "attribute vec4 b_color;\n"
        "uniform mat4 m_mvp;\n"
        "varying vec3 v_vertex;\n"
        "varying vec3 v_normal;\n"
        "varying vec4 v_color;\n"
        "void main(void) {\n"
        "    v_color = b_color;\n"
        "    v_normal = b_normal;\n"
        "    v_vertex = b_vertex;\n"
        "    gl_Position = m_mvp * vec4(b_vertex, 1.0);\n"
        "}\n";
#ifdef GL_SIMPLE_PLAT_RPI3
    const char* frag = "#version 100\n"
        "precision mediump int;\n"
        "precision mediump float;\n"
#else
    const char* frag = "#version 120\n"
#endif
        "uniform mat4 m_mv;\n"
        "varying vec4 v_color;\n"
        "varying vec3 v_normal;\n"
        "varying vec3 v_vertex;\n"
        "void main(void) {\n"
        "    vec3 normal_ms = normalize(vec3(m_mv * vec4(v_normal, 0.0)));\n"
        "    vec3 light_ms = vec3(0.0, 0.0, 0.0);\n"
        "    vec3 vert_ms = vec3(m_mv * vec4(v_vertex, 1.0));\n"
        "    vec3 stl = light_ms - vert_ms;\n"
        "    float brightness = dot(normal_ms, stl) / (length(stl) * length(normal_ms));\n"
        "    brightness = clamp(brightness, 0.0, 1.0);\n"
        "    vec3 d_color = vec3(v_color) * brightness;\n"
        "    gl_FragColor = vec4(d_color, 1.0);\n"
        "}\n";
    return gl_simple_compile_program(vert, frag, err);
}

uint32_t gl_simple_texture_shader(struct gl_simple_err* err) {
#ifdef GL_SIMPLE_PLAT_RPI3
    const char* vert = "#version 100\n"
        "precision mediump int;\n"
        "precision mediump float;\n"
#else
    const char* vert = "#version 120\n"
#endif
        "attribute vec3 b_vertex;\n"
        "attribute vec3 b_normal;\n"
        "attribute vec2 b_uv;\n"
        "uniform mat4 m_mvp;\n"
        "varying vec3 v_vertex;\n"
        "varying vec3 v_normal;\n"
        "varying vec2 v_uv;\n"
        "void main(void) {\n"
        "    v_vertex = b_vertex;\n"
        "    v_normal = b_normal;\n"
        "    v_uv = b_uv;\n"
        "    gl_Position = m_mvp * vec4(b_vertex, 1.0);\n"
        "}\n";

#ifdef GL_SIMPLE_PLAT_RPI3
    const char* frag = "#version 100\n"
        "precision mediump int;\n"
        "precision mediump float;\n"
#else
    const char* frag = "#version 120\n"
#endif
        "uniform mat4 m_mv;\n"
        "varying vec3 v_vertex;\n"
        "varying vec3 v_normal;\n"
        "uniform sampler2D s_tex;\n"
        "varying vec2 v_uv;\n"
        "void main(void) {\n"
        "    vec3 normal_ms = normalize(vec3(m_mv * vec4(v_normal, 0.0)));\n"
        "    vec3 light_ms = vec3(0.0, 0.0, 0.0);\n"
        "    vec3 vert_ms = vec3(m_mv * vec4(v_vertex, 1.0));\n"
        "    vec3 stl = light_ms - vert_ms;\n"
        "    float brightness = dot(normal_ms, stl) / (length(stl) * length(normal_ms));\n"
        "    brightness = clamp(brightness, 0.0, 1.0);\n"
        "    gl_FragColor = texture2D(s_tex, v_uv) * brightness;\n"
        "}\n";
    return gl_simple_compile_program(vert, frag, err);
}

void gl_simple_delete_buffer(uint32_t* gl_id) {
    glDeleteBuffers(1, gl_id);
}
