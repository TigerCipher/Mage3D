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
 * File Name: indexbuffer.cpp
 * Date File Created: 9/20/2020 at 9:45 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "IndexBuffer.h"
#include "GraphicsException.h"
#include "BindableCodex.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const list<ushort>& indices) :
        IndexBuffer(gfx, "?", indices)
{
    
}


IndexBuffer::IndexBuffer(Graphics& gfx, std::string tag, const list<ushort>& indices) :
    mTag(tag),
    mCount(static_cast<UINT>(indices.size()))
{
    DEBUG_INFO(gfx);

    D3D11_BUFFER_DESC ibd = { };
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = static_cast<UINT>(mCount * sizeof(ushort));
    ibd.StructureByteStride = sizeof(ushort);
    D3D11_SUBRESOURCE_DATA isd = { };
    isd.pSysMem = indices.data();
    GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&ibd, &isd, &mBuffer));
}

void IndexBuffer::bind(Graphics& gfx) noexcept
{
    getContext(gfx)->IASetIndexBuffer(mBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}


SharedPtr<IndexBuffer> IndexBuffer::resolve(Graphics& gfx, const std::string& tag, const list<ushort>& indices)
{
    assert(tag != "?");
    return BindableCodex::resolve<IndexBuffer>(gfx, tag, indices);
}


std::string IndexBuffer::getUID() const noexcept
{
    return generateUID_(mTag);
}

std::string IndexBuffer::generateUID_(const std::string& tag)
{
    using namespace std::string_literals;
    return typeid(IndexBuffer).name() + "#"s + tag;
}
