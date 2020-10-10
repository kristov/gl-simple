#include <gl-simple-base.h>
#include <gl-simple-rcsvl.h>

void gl_simple_draw_rcsvl(struct gl_simple_rcsvl* render, struct gl_simple_m* matrix) {
    GLuint shader_id = (GLuint)render->shader_id;
    glUseProgram(shader_id);
    gl_simple_print_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->vertex_id);
    GLuint b_vertex = glGetAttribLocation(shader_id, "b_vertex");
    glVertexAttribPointer(b_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_vertex);
    gl_simple_print_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)render->light_id);
    GLuint b_light = glGetAttribLocation(shader_id, "b_light");
    glVertexAttribPointer(b_light, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_light);
    gl_simple_print_error(render->err);

    GLuint f_rgba = glGetUniformLocation(shader_id, "f_rgba");
    glUniform4f(f_rgba, render->r, render->g, render->b, render->a);
    gl_simple_print_error(render->err);

    GLuint m_mvp = glGetUniformLocation(shader_id, "m_mvp");
    glUniformMatrix4fv(m_mvp, 1, GL_FALSE, matrix->mvp);
    gl_simple_print_error(render->err);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)render->index_id);
    glDrawElements(GL_TRIANGLES, (GLuint)render->nr_indexes, GL_UNSIGNED_SHORT, NULL);
    gl_simple_print_error(render->err);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    gl_simple_print_error(render->err);
}

uint32_t gl_simple_shader_rcsvl(struct gl_simple_err* err) {
#ifdef GL_SIMPLE_PLAT_RPI3
    const char* vert = "#version 100\n"
        "precision mediump int;\n"
        "precision mediump float;\n"
#else
    const char* vert = "#version 120\n"
#endif
        "attribute vec3 b_vertex;\n"
        "attribute float b_light;\n"
        "uniform vec4 f_rgba;\n"
        "uniform mat4 m_mvp;\n"
        "varying vec4 v_color;\n"
        "varying float v_light;\n"
        "void main(void) {\n"
        "    v_color = f_rgba;\n"
        "    v_light = b_light;\n"
        "    gl_Position = m_mvp * vec4(b_vertex, 1.0);\n"
        "}\n";
#ifdef GL_SIMPLE_PLAT_RPI3
    const char* frag = "#version 100\n"
        "precision mediump int;\n"
        "precision mediump float;\n"
#else
    const char* frag = "#version 120\n"
#endif
        "varying vec4 v_color;\n"
        "varying float v_light;\n"
        "void main(void) {\n"
        "    vec3 d_color = vec3(v_color) * v_light;\n"
        "    gl_FragColor = vec4(d_color, 1.0);\n"
        "}\n";
    return gl_simple_compile_program(vert, frag, err);
}
