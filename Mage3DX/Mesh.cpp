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
 * File Name: Mesh.cpp
 * Date File Created: 10/5/2020 at 4:42 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "Mesh.h"
#include "Bindables.h"

Mesh::Mesh(Graphics& gfx, list<UniquePtr<Bindable> > binds)
{
	if(!isInitialized())
	{
		addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	for (auto & b : binds)
	{
		if(auto p = dynamic_cast<IndexBuffer*>(b.get()))
		{
			addIndexBuffer(UniquePtr<IndexBuffer>(p));
			b.release();
		}else
		{
			addBind(std::move(b));
		}
	}

	addBind(createScope<TransformConstantBuffer>(gfx, *this));
}

void Mesh::render(Graphics& gfx, mat4f accumulatedTransform) const noexcept(!MAGE_DEBUG)
{
	dx::XMStoreFloat4x4(&mTransform, accumulatedTransform);
	__super::render(gfx);
}
