#version 430 core


layout(location = 0) in vec3 pos;
layout (location = 1) in vec4 color;

out vec4 fragColor;

uniform mat4 rotation;

void main()
{
    vec4 vert = vec4(pos, 1.0);
    gl_Position = vert * rotation;
    fragColor = color;
}