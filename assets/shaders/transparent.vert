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
 * File Name: transparent.vert.glsl
 * Date File Created: 8/22/2020 at 9:20 PM
 * Author: Matt
 */

#version 430 core

#include <vertex_attribs.glh>

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 fragTexCoords;

void main()
{
    vec4 vert = vec4(position, 1.0);
    gl_Position = projection * view * model * vert;
    fragTexCoords = vec2(texCoord.x, 1.0 - texCoord.y);
}
