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
#include <glm/glm.hpp>

namespace mage
{

	struct Position
	{
		float x;
		float y;
		float z;

		mage3d_EXPORT Position(float _x, float _y, float _z);
		mage3d_EXPORT Position(glm::vec3 pos);
	};

	struct ColorRGB8
	{
		float r;
		float g;
		float b;
		float a;

		mage3d_EXPORT ColorRGB8(float _r, float _g, float _b, float _a = 1.0f);
		mage3d_EXPORT ColorRGB8(glm::vec4 color);
	};


	struct Vertex
	{
		Position pos;
		ColorRGB8 color;
		mage3d_EXPORT Vertex(glm::vec3 _pos = glm::vec3(0, 0, 0),
							 glm::vec4 _color = glm::vec4(1, 1, 1, 1));

		mage3d_EXPORT void setPos(float x, float y, float z);
		mage3d_EXPORT void setColor(float r, float g, float b, float a = 1.0f);

		mage3d_EXPORT glm::vec3 posVec();
		mage3d_EXPORT glm::vec4 colorVec();

	};
}


#endif //MAGE3D_VERTEX_H
