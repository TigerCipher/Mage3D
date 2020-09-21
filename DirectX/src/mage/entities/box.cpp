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
 * Date File Created: 9/20/2020 at 11:01 PM
 * Author: Matt
 */

#include "mage/entities/box.h"
#include "mage/graphics/bindables.h"

mage::Box::Box(mage::Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
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
    struct Vertex
    {
        struct
        {
            float x;
            float y;
            float z;
        } pos;
    };
    const std::vector<Vertex> vertices =
            {
                    { -1.0f, -1.0f, -1.0f },
                    { 1.0f,  -1.0f, -1.0f },
                    { -1.0f, 1.0f,  -1.0f },
                    { 1.0f,  1.0f,  -1.0f },
                    { -1.0f, -1.0f, 1.0f },
                    { 1.0f,  -1.0f, 1.0f },
                    { -1.0f, 1.0f,  1.0f },
                    { 1.0f,  1.0f,  1.0f },
            };
    addBindable(createScope<VertexBuffer>(gfx, vertices));

    auto pvs = std::make_unique<VertexShader>(gfx, L"basicVS.cso");
    auto pvsbc = pvs->getBytecode();
    addBindable(std::move(pvs));

    addBindable(createScope<PixelShader>(gfx, L"basicPS.cso"));

    const std::vector<unsigned short> indices =
            {
                    0, 2, 1, 2, 3, 1,
                    1, 3, 5, 3, 7, 5,
                    2, 6, 3, 3, 6, 7,
                    4, 5, 7, 4, 7, 6,
                    0, 4, 2, 2, 4, 6,
                    0, 1, 4, 1, 5, 4
            };
    addIndexBuffer(createScope<IndexBuffer>(gfx, indices));

    struct ConstantBuffer2
    {
        struct
        {
            float r;
            float g;
            float b;
            float a;
        } face_colors[6];
    };
    const ConstantBuffer2 cb2 =
            {
                    {
                            { 1.0f, 0.0f, 1.0f },
                            { 1.0f, 0.0f, 0.0f },
                            { 0.0f, 1.0f, 0.0f },
                            { 0.0f, 0.0f, 1.0f },
                            { 1.0f, 1.0f, 0.0f },
                            { 0.0f, 1.0f, 1.0f },
                    }
            };
    addBindable(createScope<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
            {
                    { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
    addBindable(createScope<InputLayout>(gfx, ied, pvsbc));

    UniquePtr<Topology> t = createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    addBindable(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    addBindable(createScope<TransformConstantBuffer>(gfx, *this));
}

void mage::Box::update(float delta) noexcept
{
    roll += droll * delta;
	pitch += dpitch * delta;
	yaw += dyaw * delta;
	theta += dtheta * delta;
	phi += dphi * delta;
	chi += dchi * delta;
}

mat4f mage::Box::getTransformMatrix() const noexcept
{
    return dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
		dx::XMMatrixTranslation( r,0.0f,0.0f ) *
		dx::XMMatrixRotationRollPitchYaw( theta,phi,chi ) *
		dx::XMMatrixTranslation( 0.0f,0.0f,20.0f );
}
