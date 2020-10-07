#include <gl-simple-base.h>
#include <gl-simple-rt.h>

void gl_simple_draw_rt(struct gl_simple_rt* render, struct gl_simple_m* matrix) {
    GLuint shader_id = (GLuint)render->shader_id;
    glUseProgram(shader_id);
    gl_simple_print_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->vertex_id);
    GLuint b_vertex = glGetAttribLocation(shader_id, "b_vertex");
    glVertexAttribPointer(b_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_vertex);
    gl_simple_print_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->normal_id);
    GLuint b_normal = glGetAttribLocation(shader_id, "b_normal");
    glVertexAttribPointer(b_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_normal);
    gl_simple_print_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->uv_id);
    GLuint b_uv = glGetAttribLocation(shader_id, "b_uv");
    glVertexAttribPointer(b_uv, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_uv);
    gl_simple_print_error(render->err);

    GLuint s_tex = glGetUniformLocation(shader_id, "s_tex");
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(s_tex, 1);
    glBindTexture(GL_TEXTURE_2D, (GLuint)render->texture_id);
    gl_simple_print_error(render->err);

    GLuint m_mvp = glGetUniformLocation(shader_id, "m_mvp");
    glUniformMatrix4fv(m_mvp, 1, GL_FALSE, matrix->mvp);
    gl_simple_print_error(render->err);

    GLuint m_mv = glGetUniformLocation(shader_id, "m_mv");
    glUniformMatrix4fv(m_mv, 1, GL_FALSE, matrix->mv);
    gl_simple_print_error(render->err);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)render->index_id);
    glDrawElements(GL_TRIANGLES, (GLuint)render->nr_indexes, GL_UNSIGNED_SHORT, NULL);
    gl_simple_print_error(render->err);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    gl_simple_print_error(render->err);
}

uint32_t gl_simple_shader_rt(struct gl_simple_err* err) {
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
