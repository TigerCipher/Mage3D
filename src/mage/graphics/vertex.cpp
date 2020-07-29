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
 * File Name: vertex.cpp
 * Date File Created: 7/27/2020 at 12:43 AM
 * Author: Matt
 */

#include "mage/graphics/vertex.h"

mage::Position::Position(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

mage::Position::Position(glm::vec3 pos)
{
	x = pos.x;
	y = pos.y;
	z = pos.z;
}

mage::ColorRGB8::ColorRGB8(float _r, float _g, float _b, float _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

mage::ColorRGB8::ColorRGB8(glm::vec4 color)
{
	r = color.x;
	g = color.y;
	b = color.z;
	a = color.w;
}

void mage::Vertex::setColor(float r, float g, float b, float a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void mage::Vertex::setPos(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

glm::vec3 mage::Vertex::posVec()
{
	return glm::vec3(pos.x, pos.y, pos.z);
}

glm::vec4 mage::Vertex::colorVec()
{
	return glm::vec4(color.r, color.g, color.b, color.a);
}

mage::Vertex::Vertex(glm::vec3 _pos, glm::vec4 _color, glm::vec2 _coords) :
pos(_pos),
color(_color),
coords(_coords)
{

}

mage::Vertex::Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a):
pos(x, y, z),
color(r, g, b, a),
coords(u, v)
{

}

void mage::Vertex::setTexCoords(float u, float v)
{
	coords.u = u;
	coords.v = v;
}

glm::vec2 mage::Vertex::texCoords()
{
	return glm::vec2(coords.u, coords.v);
}

mage::TexCoords::TexCoords(float _u, float _v)
{
	u = _u;
	v = _v;
}

mage::TexCoords::TexCoords(glm::vec2 coords)
{
	u = coords.x;
	v = coords.y;
}
