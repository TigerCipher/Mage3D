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
 * File Name: Vertex.cpp
 * Date File Created: 1/22/2021 at 10:25 PM
 * Author: Matt
 */
#include "Vertex.h"


const char* Attribute::getCode() const noexcept
{
	switch(mType)
	{
	case POSITION2D: return Map<POSITION2D>::CODE;
	case POSITION3D: return Map<POSITION3D>::CODE;
	case TEXTURE2D: return Map<TEXTURE2D>::CODE;
	case NORMAL: return Map<NORMAL>::CODE;
	case COLOR3F: return Map<COLOR3F>::CODE;
	case COLOR4F: return Map<COLOR4F>::CODE;
	case COLORARGB: return Map<COLORARGB>::CODE;
	default: return "Invalid";
	}
}

std::string VertexLayout::getCode() const noexcept(!MAGE_DEBUG)
{
	std::string code;
	for(const auto& attr : mAttribs)
	{
		code += attr.getCode();
	}

	return code;
}
