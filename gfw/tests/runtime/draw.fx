#version 120

#if defined(GFW_SHADER_STAGE_VERTEX)

    attribute vec2 POSITION0;
    attribute vec3 COLOR0;

    varying   vec4 out_color;

    void main()
    {
        gl_Position = vec4(POSITION0, 0.0f, 1.0f);
        out_color   = vec4(COLOR0, 1.0f);
    }

#elif defined(GFW_SHADER_STAGE_PIXEL)

    varying vec4 out_color;

    void main()
    {
        gl_FragColor = out_color;
    }

#endif
