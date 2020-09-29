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

## Make a model and projection matrix

    float
    struct gl_simple_m matrix;

## Somewhere in a render loop render this

    gl_simple_draw_mesh_color(&render, &matrix);

