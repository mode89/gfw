#version 120

varying float   ps_fHeight;

void main()
{
    gl_FragColor = vec4(ps_fHeight, 0.0f, 0.0f, 1.0f);
}
