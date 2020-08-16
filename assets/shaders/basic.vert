#version 430 core

#include <vertex_attribs.glh>

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 vert = vec4(position, 1.0);
    gl_Position = projection * view * model * vert;
}