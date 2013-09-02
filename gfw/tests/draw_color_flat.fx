#version 120

#if defined(GFW_SHADER_STAGE_VERTEX)

    attribute vec2 POSITION0;

    flat varying vec4 o_Color;

    void main()
    {
        gl_Position = vec4(POSITION0, 0.0f, 1.0f);
        o_Color     = vec4(POSITION0.xy * vec2(0.5f, 0.5f) + vec2(0.5f, 0.5f), 0.0f, 1.0f);
    }

#elif defined(GFW_SHADER_STAGE_PIXEL)

    flat varying vec4 o_Color;

    void main()
    {
        gl_FragColor = o_Color;
    }

#endif
