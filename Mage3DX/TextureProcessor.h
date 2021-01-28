/*
 * Mage3DX
 * Copyright (C) 2021 Blue Moon Development. All rights reserved.
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
 * File Name: TextureProcessor.h
 * Date File Created: 1/26/2021 at 11:25 PM
 * Author: Matt
 */


#pragma once

#include "MathHelper.h"
#include "Color.h"

class TextureProcessor
{
public:
	static void rotateXAxis(const std::string& pathSrc, const std::string& pathDest);

	static void rotateXAxis(const std::string& pathSrc)
	{
		rotateXAxis(pathSrc, pathSrc);
	}
private:

	static vec colorToVector(Color col);
	static Color vectorToColor(vec n);
};
