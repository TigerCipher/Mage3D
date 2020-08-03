#version 430 core

out vec4 color;

in vec3 fragPos;
in vec4 fragColor;
in vec3 fragNormal;
in vec2 fragTexCoord;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D tex;

void main()
{
    float ambientIntensity = 0.1;
    vec3 ambient = ambientIntensity * lightColor;

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularIntensity = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularIntensity * spec * lightColor;

    vec3 finalLight = (ambient + diffuse + specular);
    color = texture(tex, fragTexCoord) * vec4(finalLight, 1.0) * fragColor;
}