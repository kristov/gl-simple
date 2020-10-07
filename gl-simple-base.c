#include <gl-simple-base.h>
#include <gl-matrix.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

void gl_simple_print_error_int(char *file, int line, struct gl_simple_err* err) {
    if (!err) {
        printf("no err\n");
        return;
    }
    GLenum glErr = glGetError();
    if (!glErr) {
        return;
    }
    char* log = malloc(sizeof(char) * 256);
    if (!log) {
        err->callback(err->data, "LOG MALLOC ERROR");
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
    err->callback(err->data, log);
    free(log);
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
    float right = ((float)w / (float)h) * top;
    mat4_frustum(matrix->p, -right, right, -top, top, near_z, far_z);
    gl_simple_matrix_update(matrix);
}

static void shader_err(GLuint obj, struct gl_simple_err* err) {
    int32_t length;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, (GLint*)&length);
    char* log = malloc(length);
    if (!log) {
        err->callback(err->data, "LOG MALLOC ERROR");
    }
    glGetShaderInfoLog(obj, length, NULL, log);
    err->callback(err->data, log);
    free(log);
}

static void program_err(GLuint obj, struct gl_simple_err* err) {
    int32_t length;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, (GLint*)&length);
    char* log = malloc(length);
    if (!log) {
        err->callback(err->data, "LOG MALLOC ERROR");
    }
    glGetProgramInfoLog(obj, length, NULL, log);
    err->callback(err->data, log);
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

void gl_simple_delete_buffer(uint32_t* gl_id) {
    glDeleteBuffers(1, gl_id);
}
