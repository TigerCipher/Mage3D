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
 * File Name: SolidSphere.cpp
 * Date File Created: 9/26/2020 at 4:14 PM
 * Author: Matt
 */
#include "SolidSphere.h"

#include "Primitives.h"
#include "Bindables.h"
#include "GraphicsException.h"

mage::SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	if (!isInitialized())
	{
		struct Vertex
		{
			vec3f pos;
		};

		auto model = Sphere::make<Vertex>();
		model.transform(dx::XMMatrixScaling(radius, radius, radius));
		addBindable(createScope<VertexBuffer>(gfx, model.vertices));
		addIndexBuffer(createScope<IndexBuffer>(gfx, model.indices));

		auto pvs = createScope<VertexShader>(gfx, L"solidVS.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(createScope<PixelShader>(gfx, L"solidPS.cso"));

		struct ColorConst
		{
			vec3f color = { 1.0f, 1.0f, 1.0f };
			float padding;
		} colConst;

		addStaticBind(createScope<PixelConstantBuffer<ColorConst>>(gfx, colConst));

		const list<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		addStaticBind(createScope<InputLayout>(gfx, ied, pvsbc));
		addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}else
	{
		setIndexStatic();
	}

	addBindable(createScope<TransformConstantBuffer>(gfx, *this));
}


mat4f mage::SolidSphere::getTransformMatrix() const noexcept
{
	return dx::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
}
