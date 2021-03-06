#include <gl-simple-base.h>
#include <gl-simple-rc.h>

void gl_simple_draw_rc(struct gl_simple_rc* render, struct gl_simple_m* matrix) {
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

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->color_id);
    GLuint b_color = glGetAttribLocation(shader_id, "b_color");
    glVertexAttribPointer(b_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_color);
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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    gl_simple_print_error(render->err);
}

uint32_t gl_simple_shader_rc(struct gl_simple_err* err) {
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
        "    vec3 vertex_ms = vec3(m_mv * vec4(v_normal, 1.0));\n"
        "    vec3 normal_ms = vec3(m_mv * vec4(v_normal, 0.0));\n"
        "    vec3 light_v = normalize(vec3(0.0, 0.0, 0.0) - vertex_ms);\n"
        "    float brightness = max(dot(normal_ms, light_v), 0.1);\n"
        "    vec3 d_color = vec3(v_color) * brightness;\n"
        "    gl_FragColor = vec4(d_color, 1.0);\n"
        "}\n";
    return gl_simple_compile_program(vert, frag, err);
}
