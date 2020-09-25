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
 * File Name: pyramid.cpp
 * Date File Created: 9/21/2020 at 11:44 PM
 * Author: Matt
 */

#include "mage/entities/pyramid.h"
#include "mage/graphics/bindables.h"
#include "mage/graphics/primitives.h"

mage::Pyramid::Pyramid(mage::Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
                       std::uniform_real_distribution<float>& ddist,
                       std::uniform_real_distribution<float>& odist,
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
        struct Vertex
        {
            vec3f pos;
            struct
            {
                ubyte r;
                ubyte g;
                ubyte b;
                ubyte a;
            } color;
        };
        auto model = Cone::makeTesselated<Vertex>(4);
        model.vertices[ 0 ].color = { 255, 255, 0 };
        model.vertices[ 1 ].color = { 255, 255, 0 };
        model.vertices[ 2 ].color = { 255, 255, 0 };
        model.vertices[ 3 ].color = { 255, 255, 0 };
        model.vertices[ 4 ].color = { 255, 255, 80 };
        model.vertices[ 5 ].color = { 255, 10, 0 };
        model.transform(dx::XMMatrixScaling(1.0f, 1.0f, 0.7f));

        addStaticBind(createScope<VertexBuffer>(gfx, model.vertices));

        auto pvs = createScope<VertexShader>(gfx, L"basicColorBlendVS.cso");
        auto pvsbc = pvs->getBytecode();
        addStaticBind(std::move(pvs));

        addStaticBind(createScope<PixelShader>(gfx, L"basicColorBlendPS.cso"));

        addStaticIndexBuffer(createScope<IndexBuffer>(gfx, model.indices));


        const list<D3D11_INPUT_ELEMENT_DESC> ied =
                {
                        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        {
                          "Color",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, sizeof(float) *
                                                                         3, D3D11_INPUT_PER_VERTEX_DATA, 0
                        },
                };
        addStaticBind(createScope<InputLayout>(gfx, ied, pvsbc));

        addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    } else
    {
        setIndexStatic();
    }
    addBindable(createScope<TransformConstantBuffer>(gfx, *this));
}


mat4f mage::Pyramid::getTransformMatrix() const noexcept
{
    return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
           dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
           dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}

void mage::Pyramid::update(float delta) noexcept
{
    roll += droll * delta;
    pitch += dpitch * delta;
    yaw += dyaw * delta;
    theta += dtheta * delta;
    phi += dphi * delta;
    chi += dchi * delta;
}

