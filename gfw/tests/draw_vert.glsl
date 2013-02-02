#version 120

attribute vec2 pos;
attribute vec3 color;

varying   vec4 out_color;

void main()
{
    gl_Position = vec4(pos, 0.0f, 1.0f);
    out_color   = vec4(color, 1.0f);
}
