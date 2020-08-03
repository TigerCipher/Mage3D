#version 430 core


in vec4 fragColor;
in vec2 fragTexCoord;

out vec4 color;


uniform sampler2D tex;

void main()
{
    color = texture(tex, fragTexCoord) * fragColor;
}