/*
 * Mage3DX
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
 * File Name: VertexBufferBindable.cpp
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "VertexBufferBindable.h"
#include "BindableCodex.h"

VertexBufferBindable::VertexBufferBindable(Graphics& gfx, const std::string& tag, const VertexBuffer& vData) :
mTag(tag),
mStride(static_cast<UINT>(vData.getLayout().size()))
{
	DEBUG_INFO(gfx);

	D3D11_BUFFER_DESC bd = { };
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = static_cast<UINT>(vData.sizeInBytes());
	bd.StructureByteStride = mStride;
	D3D11_SUBRESOURCE_DATA sd = { };
	sd.pSysMem = vData.getData();

	GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&bd, &sd, &mBuffer));
}

VertexBufferBindable::VertexBufferBindable(Graphics& gfx, const VertexBuffer& vData) :
	VertexBufferBindable(gfx, "?", vData)
{

}

void VertexBufferBindable::bind(Graphics& gfx) noexcept
{
	static const UINT offset = 0;
	getContext(gfx)->IASetVertexBuffers(0, 1, mBuffer.GetAddressOf(), &mStride, &offset);
}



SharedPtr<VertexBufferBindable> VertexBufferBindable::resolve(Graphics& gfx, const std::string& tag, const VertexBuffer& vData)
{
	assert(tag != "?");
	return BindableCodex::resolve<VertexBufferBindable>(gfx, tag, vData);
}

std::string VertexBufferBindable::getUID() const noexcept
{
	return generateUID(mTag);
}

std::string VertexBufferBindable::generateUID_(const std::string& tag)
{
	using namespace std::string_literals;
	return typeid(VertexBufferBindable).name() + "#"s + tag;
}
