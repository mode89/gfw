#version 120

#if defined(GFW_SHADER_STAGE_VERTEX)

attribute vec2 POSITION0;

void main()
{
    gl_Position = vec4(POSITION0, 0.0f, 1.0f);
}

#elif defined(GFW_SHADER_STAGE_PIXEL)

void main()
{
    gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

#endif
