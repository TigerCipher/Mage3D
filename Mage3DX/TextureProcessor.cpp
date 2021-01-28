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
 * File Name: TextureProcessor.cpp
 * Date File Created: 1/26/2021 at 11:25 PM
 * Author: Matt
 */
#include "TextureProcessor.h"
#include "Texture.h"


void TextureProcessor::rotateXAxis(const std::string& pathSrc, const std::string& pathDest)
{
	const auto rot = rotateXMatrix(PI);
	auto texIn = Texture::loadFromFile(pathSrc);

	const auto pixels = texIn.getWidth() * texIn.getHeight();
	auto* beg = texIn.getBuffer();
	const auto* end = texIn.getBuffer() + pixels;

	for(auto* cur = beg; cur < end; cur++)
	{
		auto n = colorToVector(*cur);
		n = transformVector3(n, rot);
		*cur = vectorToColor(n);
	}

	texIn.save(pathDest);
}


vec TextureProcessor::colorToVector(Color col)
{
	auto n = setVector(static_cast<float>(col.getRed()),
		static_cast<float>(col.getGreen()),
		static_cast<float>(col.getBlue()),
		0.0f);

	const auto all255 = replicateVector(2.0f / 255.0f);
	n = mulVector(n, all255);
	const auto all1 = replicateVector(1.0f);
	n = subVector(n, all1);
	return n;
}

Color TextureProcessor::vectorToColor(vec n)
{
	const auto all1 = replicateVector(1.0f);
	vec out = addVector(n, all1);
	const auto all255 = replicateVector(255.0f / 2.0f);
	out = mulVector(out, all255);
	vec3f floats;
	storeVector3(&floats, out);
	return {
		static_cast<uchar>(round(floats.x)),
		static_cast<uchar>(round(floats.y)),
		static_cast<uchar>(round(floats.z))
	};
}
