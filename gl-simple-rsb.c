#include <gl-simple-rsb.h>

#include <stdio.h>

void gl_simple_init_rsb(struct gl_simple_rsb* render) {
    float verts[] = {
         100.0f, -100.0f,  100.0f,
         100.0f, -100.0f, -100.0f,
         100.0f,  100.0f,  100.0f,
         100.0f,  100.0f, -100.0f,
        -100.0f, -100.0f, -100.0f,
        -100.0f, -100.0f,  100.0f,
        -100.0f,  100.0f, -100.0f,
        -100.0f,  100.0f,  100.0f
    };
    uint16_t indexes[] = {
        0, 1, 2, 2, 1, 3,
        4, 5, 6, 6, 5, 7,
        7, 2, 6, 6, 2, 3,
        4, 1, 5, 5, 1, 0,
        5, 0, 7, 7, 0, 2,
        1, 4, 3, 3, 4, 6
    };
    render->vertex_id = gl_simple_load_float_buffer(&verts[0], 24);
    render->index_id = gl_simple_load_integer_buffer(&indexes[0], 36);
    render->nr_indexes = 36;
}

void gl_simple_draw_rsb(struct gl_simple_rsb* render, struct gl_simple_m* matrix) {
    GLuint shader_id = (GLuint)render->shader_id;
    glUseProgram(shader_id);
    glDisable(GL_DEPTH_TEST);
    gl_simple_print_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->vertex_id);
    GLuint b_vertex = glGetAttribLocation(shader_id, "b_vertex");
    glVertexAttribPointer(b_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_vertex);
    gl_simple_print_error(render->err);

    GLuint s_tex = glGetUniformLocation(shader_id, "s_tex");
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(s_tex, 1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)render->texture_id);
    gl_simple_print_error(render->err);

    GLuint m_mvp = glGetUniformLocation(shader_id, "m_mvp");
    glUniformMatrix4fv(m_mvp, 1, GL_FALSE, matrix->mvp);
    gl_simple_print_error(render->err);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)render->index_id);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
    gl_simple_print_error(render->err);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    gl_simple_print_error(render->err);
}

void gl_simple_load_cube_texture(struct gl_simple_rsb* render, uint8_t* buff, uint32_t w, uint32_t h) {
    uint32_t o = 0;
    uint32_t po = w * h * 4;
    glGenTextures(1, &render->texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, render->texture_id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buff[o]);
    o += po;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buff[o]);
    o += po;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buff[o]);
    o += po;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buff[o]);
    o += po;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buff[o]);
    o += po;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buff[o]);
    gl_simple_print_error(render->err);
}

uint32_t gl_simple_shader_rsb(struct gl_simple_err* err) {
#ifdef GL_SIMPLE_PLAT_RPI3
    const char* vert = "#version 100\n"
        "precision mediump int;\n"
        "precision mediump float;\n"
#else
    const char* vert = "#version 120\n"
#endif
        "attribute vec3 b_vertex;\n"
        "uniform mat4 m_mvp;\n"
        "varying vec3 v_pos;\n"
        "void main(void) {\n"
        "    v_pos = normalize(b_vertex);\n"
        "    gl_Position = m_mvp * vec4(b_vertex, 1.0);\n"
        "}\n";
#ifdef GL_SIMPLE_PLAT_RPI3
    const char* frag = "#version 100\n"
        "precision mediump int;\n"
        "precision mediump float;\n"
#else
    const char* frag = "#version 120\n"
#endif
        "uniform samplerCube s_tex;\n"
        "varying vec3 v_pos;\n"
        "void main(void) {\n"
        "    gl_FragColor = textureCube(s_tex, v_pos);\n"
        "}\n";
    return gl_simple_compile_program(vert, frag, err);
}
