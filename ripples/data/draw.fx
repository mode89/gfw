#version 120

#if defined(GFW_SHADER_STAGE_VERTEX)

    attribute vec4  POSITION0;

    varying   float ps_fHeight;

    void main()
    {
        gl_Position = vec4(POSITION0.xy, 0.0f, 1.0f);
        ps_fHeight  = POSITION0.z + 0.5f;
    }

#elif defined(GFW_SHADER_STAGE_PIXEL)

    varying float   ps_fHeight;

    void main()
    {
        gl_FragColor = vec4(ps_fHeight, 0.0f, 0.0f, 1.0f);
    }

#endif
