#version 430 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;

out vec3 fragPos;
out vec4 fragColor;
out vec3 fragNormal;
out vec2 fragTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 vert = vec4(position, 1.0);
    gl_Position = projection * view * model * vert;
    fragPos = vec3(model * vert);
    fragNormal = mat3(transpose(inverse(model))) * normal;
    fragColor = color;
   fragTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
    // fragTexCoord = texCoord;
}