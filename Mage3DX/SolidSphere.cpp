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
//#include "pch.h" -intellisense works better with force include being used
#include "SolidSphere.h"

#include "Primitives.h"
#include "Bindables.h"
#include "GraphicsException.h"
#include "Vertex.h"

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
		auto model = Sphere::make();
		model.transform(dx::XMMatrixScaling(radius, radius, radius));
		addBind(createRef<VertexBufferBindable>(gfx, model.vertices));
		addBind(createRef<IndexBuffer>(gfx, model.indices));

		auto pvs = createScope<VertexShader>(gfx, L"shaders\\solidVS.cso");
		auto pvsbc = pvs->getBytecode();
		addBind(std::move(pvs));

		addBind(createRef<PixelShader>(gfx, L"shaders\\solidPS.cso"));

		struct ColorConst
		{
			vec3f color = { 1.0f, 1.0f, 1.0f };
			float padding;
		} colConst;

		addBind(createRef<PixelConstantBuffer<ColorConst>>(gfx, colConst));

		addBind(createRef<InputLayout>(gfx, model.vertices.getLayout().getD3dLayout(), pvsbc));
		addBind(createRef<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	addBind(createRef<TransformConstantBuffer>(gfx, *this));
}


mat4f SolidSphere::getTransformMatrix() const noexcept
{
	return dx::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
}
