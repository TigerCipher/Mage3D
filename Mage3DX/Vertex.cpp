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

size_t Attribute::sizeOf(const AttributeType type) noexcept(!MAGE_DEBUG)
{
	switch (type)
	{
	case POSITION2D: return sizeof(Map<POSITION2D>::sysType);
	case POSITION3D: return sizeof(Map<POSITION3D>::sysType);
	case TEXTURE2D: return sizeof(Map<TEXTURE2D>::sysType);
	case NORMAL: return sizeof(Map<NORMAL>::sysType);
	case COLOR3F: return sizeof(Map<COLOR3F>::sysType);
	case COLOR4F: return sizeof(Map<COLOR4F>::sysType);
	case COLORARGB: return sizeof(Map<COLORARGB>::sysType);
	default:
		LOG_ASSERT(false, "Invalid attribute type");
		return 0;
	}
}

D3D11_INPUT_ELEMENT_DESC Attribute::getDesc() const noexcept(!MAGE_DEBUG)
{
	switch (mType)
	{
	case POSITION2D: return getDesc<POSITION2D>(getOffset());
	case POSITION3D: return getDesc<POSITION3D>(getOffset());
	case TEXTURE2D: return getDesc<TEXTURE2D>(getOffset());
	case NORMAL: return getDesc<NORMAL>(getOffset());
	case COLOR3F: return getDesc<COLOR3F>(getOffset());
	case COLOR4F: return getDesc<COLOR4F>(getOffset());
	case COLORARGB: return getDesc<COLORARGB>(getOffset());
	default:
		LOG_ASSERT(false, "Invalid attribute type");
		return { "INVALID", 0, DXGI_FORMAT_UNKNOWN,
				 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	}
}

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

VertexLayout& VertexLayout::append(AttributeType type) noexcept(!MAGE_DEBUG)
{
	mAttribs.emplace_back(type, size());
	return *this;
}

list<D3D11_INPUT_ELEMENT_DESC> VertexLayout::getD3dLayout() const noexcept(!MAGE_DEBUG)
{
	list< D3D11_INPUT_ELEMENT_DESC> desc;
	for (const auto& a : mAttribs)
	{
		desc.push_back(a.getDesc());
	}
	return desc;
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
