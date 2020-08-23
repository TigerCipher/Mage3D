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

struct Material
{
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_emission;
    float shininess;
};
uniform Material material;

#include <lighting.glh>

uniform vec3 viewPos;

void main()
{
    vec4 emission = texture(material.texture_emission, fragTexCoord);

    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec4 result = calculateDirectionLight(directionLight, material, norm, viewDir);

    for(int i = 0; i < NUM_PNT_LIGHTS; i++)
        result += calculatePointLight(pointLights[i], material, norm, fragPos, viewDir);

    result += calculateSpotLight(spotLight, material, norm, fragPos, viewDir);
    result += emission;

    color = result;
}
