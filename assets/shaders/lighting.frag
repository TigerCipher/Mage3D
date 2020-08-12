/*
 * Mage3D
 * Copyright (C) 2020 Blue Moon Development. All rights reserved.
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: team@bluemoondev.org
 * 
 * File Name: lighting.frag.glsl
 * Date File Created: 8/5/2020 at 6:52 PM
 * Author: Matt
 */

#version 430 core

out vec4 color;

in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragLightPos;

struct Material
{
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_emission;
    float shininess;
};

struct Light
{
//vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;


void main()
{
    vec3 emission = vec3(texture(material.texture_emission, fragTexCoord));
    vec3 ambient = vec3(texture(material.texture_diffuse, fragTexCoord)) * light.ambient;

    vec3 norm = normalize(fragNormal);
    vec3 lightToObj = normalize(fragLightPos - fragPos);

    float diff = max(dot(norm, lightToObj), 0);
    vec3 diffuse = light.diffuse * vec3(texture(material.texture_diffuse, fragTexCoord)) * diff;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightToObj, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, fragTexCoord));



    vec3 result = (ambient + diffuse + specular + emission);
    color = vec4(result, 1.0);
}
