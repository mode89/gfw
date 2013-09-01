#version 120

#if defined(GFW_SHADER_STAGE_VERTEX)

    attribute vec2 pos;
    attribute vec3 color;

    varying   vec4 out_color;

    void main()
    {
        gl_Position = vec4(pos, 0.0f, 1.0f);
        out_color   = vec4(color, 1.0f);
    }

#elif defined(GFW_SHADER_STAGE_PIXEL)

    varying vec4 out_color;

    void main()
    {
        gl_FragColor = out_color;
    }

#endif
