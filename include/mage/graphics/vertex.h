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
 * File Name: vertex.h
 * Date File Created: 7/27/2020 at 12:43 AM
 * Author: Matt
 */

#ifndef MAGE3D_VERTEX_H
#define MAGE3D_VERTEX_H

#include "mage3d_exported.h"
#include <bmd/types.h>
#include <glm/glm.hpp>
#include <ostream>

namespace mage
{

	struct Position
	{
		float x;
		float y;
		float z;

		Position(float _x, float _y, float _z) :
				x(_x),
				y(_y),
				z(_z) { }

		friend std::ostream& operator<<(std::ostream& os, const Position& position)
		{
			os << "x: " << position.x << " y: " << position.y << " z: " << position.z;
			return os;
		}
	};

	// Keeping for now, but might get rid of. If color overlays are really needed, they can just be a uniform in the shader
	struct ColorRGB8
	{
		float r;
		float g;
		float b;
		float a;

		ColorRGB8(float _r, float _g, float _b, float _a = 1.0f) :
				r(_r),
				g(_g),
				b(_b),
				a(_a) { }

		friend std::ostream& operator<<(std::ostream& os, const ColorRGB8& rgb8)
		{
			os << "r: " << rgb8.r << " g: " << rgb8.g << " b: " << rgb8.b << " a: " << rgb8.a;
			return os;
		}
	};

	struct TexCoords
	{
		float u;
		float v;

		TexCoords(float _u, float _v) :
				u(_u),
				v(_v) { }

		friend std::ostream& operator<<(std::ostream& os, const TexCoords& coords)
		{
			os << "u: " << coords.u << " v: " << coords.v;
			return os;
		}
	};

	struct Normal
	{
		float x;
		float y;
		float z;

		Normal(float _x, float _y, float _z) :
				x(_x),
				y(_y),
				z(_z) { }

		friend std::ostream& operator<<(std::ostream& os, const Normal& normal)
		{
			os << "x: " << normal.x << " y: " << normal.y << " z: " << normal.z;
			return os;
		}
	};


	struct Vertex
	{
		Position pos;
		Normal normal;
		TexCoords coords;
		ColorRGB8 color;

		explicit Vertex(Position _pos, Normal _normal = Normal(0, 0, 0), TexCoords _texCoords = TexCoords(0, 0),
			   ColorRGB8 _color = ColorRGB8(1, 1, 1)) :
				pos(_pos),
				normal(_normal),
				coords(_texCoords),
				color(_color) { }

		void setPos(float x, float y, float z)
		{
			pos.x = x;
			pos.y = y;
			pos.z = z;
		}
		void setColor(float r, float g, float b, float a = 1.0f)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
		void setTexCoords(float u, float v)
		{
			coords.u = u;
			coords.v = v;
		}
		void setNormalCoords(float x, float y, float z)
		{
			normal.x = x;
			normal.y = y;
			normal.z = z;
		}


		friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
		{
			os << "pos: " << vertex.pos << " normal: " << vertex.normal << " coords: " << vertex.coords
			   << " color: " << vertex.color;
			return os;
		}
	};
}


#endif //MAGE3D_VERTEX_H
