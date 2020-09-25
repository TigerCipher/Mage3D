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
 * File Name: transformconstantbuffer.cpp
 * Date File Created: 9/20/2020 at 10:37 PM
 * Author: Matt
 */

#include "mage/graphics/bindables/transform_constant_buffer.h"


UniquePtr<mage::VertexConstantBuffer<mat4f>> mage::TransformConstantBuffer::m_vertexBuffer;

mage::TransformConstantBuffer::TransformConstantBuffer(mage::Graphics& gfx, const mage::IRenderable& parent): m_parent(parent)
{
    if(!m_vertexBuffer)
    {
        m_vertexBuffer = createScope<VertexConstantBuffer<mat4f>>(gfx);
    }
}


void mage::TransformConstantBuffer::bind(mage::Graphics& gfx) noexcept
{
    m_vertexBuffer->update(gfx, dx::XMMatrixTranspose(m_parent.getTransformMatrix() * gfx.getCamera() * gfx.getProjection()));
    m_vertexBuffer->bind(gfx);
}

