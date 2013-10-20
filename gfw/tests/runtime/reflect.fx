#version 430

#if defined(GFW_SHADER_STAGE_VERTEX)

    in  vec4    POSITION0;
    in  vec3    COLOR0;
    in  float   COLOR3;

    out gl_PerVertex
    {
        vec4 gl_Position;
    };

    out vec2    io_1;
    out float   io_2;

    uniform buf1
    {
        mat4    g_var1;
        float   g_var2;
    };

    uniform buf2
    {
        vec4    g_var3;
        vec2    g_var4;
        vec3    g_var5;
    };

    uniform float   g_var6;
    uniform mat4    g_var7;
    uniform vec3    g_var8;

    void main()
    {
        vec4 val =
            POSITION0 * g_var1 * g_var7 +
            vec4(COLOR0, 1.0f) +
            vec4(COLOR3, 0.0f, 0.0f, 1.0f) +
            vec4(g_var2, 0.0f, 0.0f, 1.0f) +
            g_var3 +
            vec4(g_var4, 0.0f, 1.0f) +
            vec4(g_var5, 1.0f) +
            vec4(g_var6, 0.0f, 0.0f, 1.0f) +
            vec4(g_var8, 1.0f);

        gl_Position = val;
        io_1 = val.xy;
        io_2 = val.x;
    }

#elif defined(GFW_SHADER_STAGE_PIXEL)

    in  vec2    io_1;
    in  float   io_2;

    uniform vec4    g_var1;
    uniform float   g_var2;

    uniform sampler2D   g_tex1;
    uniform sampler2D   g_tex2;

    void main()
    {
        gl_FragColor =
            vec4(io_1, 0.0f, 1.0f) +
            vec4(io_2, 0.0f, 0.0f, 1.0f) +
            g_var1 +
            vec4(g_var2, 0.0f, 0.0f, 1.0f) +
            texture(g_tex1, io_1) +
            texture(g_tex2, io_1);
    }

#endif
