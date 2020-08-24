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
 * File Name: screen.frag.glsl
 * Date File Created: 8/23/2020 at 2:15 PM
 * Author: Matt
 */

#version 430 core

out vec4 color;

in vec2 fragTexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 col = texture(screenTexture, fragTexCoords);
    float average = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
    color = vec4(average, average, average, 1.0);
}
