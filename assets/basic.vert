#version 430 core


layout(location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;

out vec4 fragColor;
out vec2 fragTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 vert = vec4(pos, 1.0);
    gl_Position = projection * view * model * vert;
    fragColor = color;
    fragTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}