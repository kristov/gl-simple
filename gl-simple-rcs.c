#include <gl-simple-rcs.h>

void gl_simple_draw_rcs(struct gl_simple_rcs* render, struct gl_simple_m* matrix) {
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

    GLuint f_rgba = glGetUniformLocation(shader_id, "f_rgba");
    glUniform4f(f_rgba, render->r, render->g, render->b, render->a);
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

uint32_t gl_simple_shader_rcs(struct gl_simple_err* err) {
#ifdef GL_SIMPLE_PLAT_RPI3
    const char* vert = "#version 100\n"
        "precision mediump int;\n"
        "precision mediump float;\n"
#else
    const char* vert = "#version 120\n"
#endif
        "attribute vec3 b_vertex;\n"
        "attribute vec3 b_normal;\n"
        "uniform vec4 f_rgba;\n"
        "uniform mat4 m_mvp;\n"
        "varying vec3 v_vertex;\n"
        "varying vec3 v_normal;\n"
        "varying vec4 v_color;\n"
        "void main(void) {\n"
        "    v_color = f_rgba;\n"
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
