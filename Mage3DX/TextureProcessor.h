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
#include "Texture.h"

class TextureProcessor
{
public:

	// Rotates 180 degrees
	static void rotateXAxis(const std::string& pathSrc, const std::string& pathDest);

	// Rotates 180 degrees
	static void rotateXAxis(const std::string& pathSrc)
	{
		rotateXAxis(pathSrc, pathSrc);
	}

	static void flipYNormalMap(const std::string& src, const std::string& dest);

	static void flipYForAllModelNormalMaps(const std::string& modelPath);

	static void reformatTexture(const std::string& fileName);
	static void reformatAllTextures(const std::string& modelPath);

	static void validateNormalMap(const std::string& path, const float minThreshold, const float maxThreshold);
private:

	template<typename F>
	static void transformTexture(Texture& tex, F&& func)
	{
		const auto w = tex.getWidth();
		const auto h = tex.getHeight();

		for(uint y = 0; y < h; y++)
		{
			for(uint x = 0; x < w; x++)
			{
				const auto n = colorToVector(tex.getPixel(x, y));
				tex.setPixel(x, y, vectorToColor(func(n, x, y)));
			}
		}
	}
	
	template<typename F>
	static void transformFile(const std::string& pathSrc, const std::string& pathDest, F&& func)
	{
		auto tex = Texture::loadFromFile(pathSrc);
		transformTexture(tex, func);
		tex.save(pathDest);
	}

	static vec colorToVector(const Color& col);
	static Color vectorToColor(vec n);
};
