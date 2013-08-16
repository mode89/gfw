#version 120

attribute vec4  vs_vPosition;

varying   float ps_fHeight;

void main()
{
    gl_Position = vec4(vs_vPosition.xy, 0.0f, 1.0f);
    ps_fHeight  = vs_vPosition.z + 0.5f;
}
