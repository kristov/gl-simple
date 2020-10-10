#include <gl-simple-base.h>
#include <gl-simple-rtvl.h>

void gl_simple_draw_rtvl(struct gl_simple_rtvl* render, struct gl_simple_m* matrix) {
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)render->index_id);
    glDrawElements(GL_TRIANGLES, (GLuint)render->nr_indexes, GL_UNSIGNED_SHORT, NULL);
    gl_simple_print_error(render->err);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    gl_simple_print_error(render->err);
}

uint32_t gl_simple_shader_rtvl(struct gl_simple_err* err) {
#ifdef GL_SIMPLE_PLAT_RPI3
    const char* vert = "#version 100\n"
        "precision mediump int;\n"
        "precision mediump float;\n"
#else
    const char* vert = "#version 120\n"
#endif
        "attribute vec3 b_vertex;\n"
        "attribute float b_light;\n"
        "attribute vec2 b_uv;\n"
        "uniform mat4 m_mvp;\n"
        "varying float v_light;\n"
        "varying vec2 v_uv;\n"
        "void main(void) {\n"
        "    v_light = b_light;\n"
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
        "uniform sampler2D s_tex;\n"
        "varying float v_light;\n"
        "varying vec2 v_uv;\n"
        "void main(void) {\n"
        "    gl_FragColor = texture2D(s_tex, v_uv) * v_light;\n"
        "}\n";
    return gl_simple_compile_program(vert, frag, err);
}
