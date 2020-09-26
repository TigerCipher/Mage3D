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
 * File Name: constantbuffer.h
 * Date File Created: 9/20/2020 at 9:24 PM
 * Author: Matt
 */

#ifndef MAGE3DX_CONSTANT_BUFFER_H
#define MAGE3DX_CONSTANT_BUFFER_H


//#include "pch.h"
#include "graphics_exception.h"
#include "bindable.h"

namespace mage
{
    template<typename C>
    class ConstantBuffer : public Bindable
    {
    public:
        explicit ConstantBuffer(Graphics& gfx)
        {
            DEBUG_INFO(gfx);
            D3D11_BUFFER_DESC desc;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.ByteWidth = sizeof(C);
            desc.StructureByteStride = 0;
            GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&desc, nullptr, &m_buffer));
        }

        ConstantBuffer(Graphics& gfx, const C& consts)
        {
            DEBUG_INFO(gfx);
            D3D11_BUFFER_DESC desc;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.ByteWidth = sizeof(consts);
            desc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA subData = {};
            subData.pSysMem = &consts;

            GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&desc, &subData, &m_buffer));
        }

        void update(Graphics& gfx, const C& consts)
        {
            DEBUG_INFO(gfx);

            D3D11_MAPPED_SUBRESOURCE msr;
            GFX_THROW_INFO(getContext(gfx)->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
            memcpy(msr.pData, &consts, sizeof(consts));
            getContext(gfx)->Unmap(m_buffer.Get(), 0);
        }

    protected:
        COMptr<ID3D11Buffer> m_buffer;
    };

    template<typename C>
    class VertexConstantBuffer : public ConstantBuffer<C>
    {
        using ConstantBuffer<C>::m_buffer;
        using Bindable::getContext;

    public:
        using ConstantBuffer<C>::ConstantBuffer;

        void bind(Graphics& gfx) noexcept override
        {
            getContext(gfx)->VSSetConstantBuffers(0, 1, m_buffer.GetAddressOf());
        }
    };

    template<typename C>
    class PixelConstantBuffer : public ConstantBuffer<C>
    {
        using ConstantBuffer<C>::m_buffer;
        using Bindable::getContext;

    public:
        using ConstantBuffer<C>::ConstantBuffer;

        void bind(Graphics& gfx) noexcept override
        {
            getContext(gfx)->PSSetConstantBuffers(0, 1, m_buffer.GetAddressOf());
        }
    };


}// namespace mage


#endif//MAGE3DX_CONSTANT_BUFFER_H
