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

#include "mage/graphics/indexbuffer.h"
#include "mage/debug/graphicsexception.h"

mage::IndexBuffer::IndexBuffer(mage::Graphics& gfx, const list<ushort>& indices) :
        m_count((UINT) indices.size())
{
    DEBUG_INFO(gfx);

    D3D11_BUFFER_DESC ibd = { };
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = UINT(m_count * sizeof(ushort));
    ibd.StructureByteStride = sizeof(ushort);
    D3D11_SUBRESOURCE_DATA isd = { };
    isd.pSysMem = indices.data();
    GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&ibd, &isd, &m_buffer));
}

void mage::IndexBuffer::bind(mage::Graphics& gfx) noexcept
{
    getContext(gfx)->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}
