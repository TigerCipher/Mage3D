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
 * File Name: vertexbuffer.h
 * Date File Created: 9/20/2020 at 10:41 PM
 * Author: Matt
 */

#pragma once


#include "GraphicsException.h"
#include "Bindable.h"
#include "Vertex.h"

namespace mage
{
    class VertexBuffer : public Bindable
    {
    public:
        template<typename V>
        VertexBuffer(Graphics& gfx, const list<V>& vertices) : m_stride(sizeof(V))
        {
            DEBUG_INFO(gfx);

            D3D11_BUFFER_DESC bd = {};
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.CPUAccessFlags = 0u;
            bd.MiscFlags = 0u;
            bd.ByteWidth = static_cast<UINT>(sizeof(V) * vertices.size());
            bd.StructureByteStride = m_stride;
            D3D11_SUBRESOURCE_DATA sd = {};
            sd.pSysMem = vertices.data();

            GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&bd, &sd, &m_buffer));
        }

        VertexBuffer(Graphics& gfx, const VertexData& vData) : m_stride(static_cast<UINT>(vData.getLayout().size()))
        {
			DEBUG_INFO(gfx);

			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0u;
			bd.MiscFlags = 0u;
			bd.ByteWidth = static_cast<UINT>(vData.sizeInBytes());
            bd.StructureByteStride = m_stride;
			D3D11_SUBRESOURCE_DATA sd = {};
            sd.pSysMem = vData.getData();

			GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&bd, &sd, &m_buffer));
        }

        void bind(Graphics& gfx) noexcept override;

    protected:
        UINT m_stride;
        COMptr<ID3D11Buffer> m_buffer{};
    };


}// namespace mage
