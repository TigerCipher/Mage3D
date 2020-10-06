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
 * File Name: TransformConstantBuffer.cpp
 * Date File Created: 9/28/2020 at 1:34 AM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "TransformConstantBuffer.h"


UniquePtr<VertexConstantBuffer<TransformConstantBuffer::Transforms> >
TransformConstantBuffer::sVertexBuffer;

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const IRenderable& parent, UINT slot /*= 0*/) :
	m_parent(parent)
{
	if(!sVertexBuffer)
	{
		sVertexBuffer = createScope<VertexConstantBuffer<Transforms> >(gfx, slot);
	}
}


void TransformConstantBuffer::bind(Graphics& gfx) noexcept
{
	const auto modelView = m_parent.getTransformMatrix() * gfx.getCamera();
	const Transforms t = {
		dx::XMMatrixTranspose(modelView),
		dx::XMMatrixTranspose(modelView * gfx.getProjection())
	};
	sVertexBuffer->update(gfx, t);
	sVertexBuffer->bind(gfx);
}

