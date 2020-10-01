## gl-simple - Quick'n'dirty OpenGL tool

Allows the user to quickly render simple colored and textured meshes with a single light source.

Basic usage:

## Make some vertex, normal, color and index data

    float verts[] = {
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  0.0f
    };

    float norms[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f
    };

    uint16_t indexes[] = {0, 1, 2, 1, 2, 3};

## Create a render struct and populate it

    struct gl_simple_r render;

    render.vertex_id = gl_simple_load_float_buffer(verts, 12);
    render.normal_id = gl_simple_load_float_buffer(norms, 12);
    render.color_id = gl_simple_load_float_buffer(colors, 16);
    render.index_id = gl_simple_load_integer_buffer(indexes, 6);
    render.nr_indexes = 6;

## Make a basic color shader

    render.shader_id = gl_simple_color_shader(NULL);

## Make a set of matricies

    float model[16];
    float view[16];
    float projection[16];
    float mv[16];
    float mvp[16];

## And attach them to a matrix object

    struct gl_simple_m matrix;

    matrix.m = &model[0];
    matrix.v = &view[0];
    matrix.p = &projection[0];
    matrix.mv = &mv[0];
    matrix.mvp = &mvp[0];

## Init the matricies

    gl_simple_matrix_init(&matrix);

## Do model or view transformations

    mat4_translatef(matrix.m, 0, 0, -5.0f);

## Set the perspective

    gl_simple_matrix_init_perspective(&matrix, width, height);

## Somewhere in a render loop render this

    gl_simple_draw_mesh_color(&render, &matrix);

## When you change a matrix remember to update

    mat4_translatef(matrix.m, 0, 0, -6.0f);
    gl_simple_matrix_update(&matrix);

