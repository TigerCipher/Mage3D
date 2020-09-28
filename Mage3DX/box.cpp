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
 * File Name: box.cpp
 * Date File Created: 9/25/2020 at 11:49 PM
 * Author: Matt
 */
#include "Box.h"
#include "Bindables.h"
#include "primitives.h"
#include "sampler.h"
#include "texture.h"
#include "texture_surface.h"

mage::Box::Box(mage::Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
               std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist,
               std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist,
               vec3f material) :
	DummyObject(gfx, rng, adist, ddist, odist, rdist)
{
	if (!isInitialized())
	{
		struct Vertex {
			vec3f pos;
			vec3f normal;
		};
		auto model = Cube::makeIndependent<Vertex>();
		model.setNormalsFlat();
		//model.transform( dx::XMMatrixScaling( 1.0f,1.0f,1.2f ) );

		addStaticBind(createScope<VertexBuffer>(gfx, model.vertices));

		auto pvs = createScope<VertexShader>(gfx, L"phongVS.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(createScope<PixelShader>(gfx, L"phongPS.cso"));

		addStaticIndexBuffer(createScope<IndexBuffer>(gfx, model.indices));


		const list<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		addStaticBind(createScope<InputLayout>(gfx, ied, pvsbc));

		addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	} else
	{
		setIndexStatic();
	}

	addBindable(createScope<TransformConstantBuffer>(gfx, *this));

	struct MaterialConst
	{
		alignas(16) vec3f color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;

		float padding[2];
	} colConst;
	colConst.color = material;
	addBindable(createScope<PixelConstantBuffer<MaterialConst> >(gfx, colConst, 1));

	dx::XMStoreFloat3x3(&mt, dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}


mat4f mage::Box::getTransformMatrix() const noexcept
{
	return dx::XMLoadFloat3x3(&mt) * DummyObject::getTransformMatrix();
}
mat4f mage::SkinnedBox::getTransformMatrix() const noexcept
{
	return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
	       dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}
void mage::SkinnedBox::update(float delta) noexcept
{
	roll += droll * delta;
	pitch += dpitch * delta;
	yaw += dyaw * delta;
	theta += dtheta * delta;
	phi += dphi * delta;
	chi += dchi * delta;
}

mage::SkinnedBox::SkinnedBox(mage::Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
                             std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist,
                             std::uniform_real_distribution<float>& rdist) :
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	if (!isInitialized())
	{
		struct Vertex {
			vec3f pos;
			struct {
				float u, v;
			} tex;
		};

		auto model = Cube::makeSkinned<Vertex>();

		addStaticBind(createScope<Texture>(gfx, TextureSurface::loadFromFile("assets\\textures\\cube.png")));

		addStaticBind(createScope<VertexBuffer>(gfx, model.vertices));

		addStaticBind(createScope<Sampler>(gfx));

		auto pvs = createScope<VertexShader>(gfx, L"textureVS.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(createScope<PixelShader>(gfx, L"texturePS.cso"));

		addStaticIndexBuffer(createScope<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		addStaticBind(createScope<InputLayout>(gfx, ied, pvsbc));

		addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	} else
	{
		setIndexStatic();
	}

	addBindable(createScope<TransformConstantBuffer>(gfx, *this));
}
