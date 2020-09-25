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
 * File Name: sheet.cpp
 * Date File Created: 9/23/2020 at 10:41 PM
 * Author: Matt
 */

#include "mage/entities/sheet.h"
#include "mage/graphics/bindables.h"
#include "mage/graphics/primitives.h"
#include "mage/graphics/sampler.h"
#include "mage/graphics/texture.h"
#include "mage/graphics/texture_surface.h"

mage::Sheet::Sheet(mage::Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
                   std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist,
                   std::uniform_real_distribution<float>& rdist) :
    r(rdist(rng)),
    droll(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dphi(odist(rng)), dtheta(odist(rng)), dchi(odist(rng)),
    chi(adist(rng)), theta(adist(rng)), phi(adist(rng))
{
    if (!isInitialized())
    {
        struct Vertex {
            vec3f pos;
            struct {
                float u, v;
            } tex;
        };

        auto model = Plane::make<Vertex>();
        model.vertices[ 0 ].tex = {0.0f, 0.0f};
        model.vertices[ 1 ].tex = {1.0f, 0.0f};
        model.vertices[ 2 ].tex = {0.0f, 1.0f};
        model.vertices[ 3 ].tex = {1.0f, 1.0f};

        addStaticBind(createScope<Texture>(gfx, TextureSurface::loadFromFile("assets\\textures\\test.png")));

        addStaticBind(createScope<VertexBuffer>(gfx, model.vertices));

        addStaticBind(createScope<Sampler>(gfx));

        auto pvs = createScope<VertexShader>(gfx, L"textureVS.cso");
        auto pvsbc = pvs->getBytecode();
        addStaticBind(std::move(pvs));

        addStaticBind(createScope<PixelShader>(gfx, L"texturePS.cso"));

        addStaticIndexBuffer(createScope<IndexBuffer>(gfx, model.indices));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
                {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        addStaticBind(createScope<InputLayout>(gfx, ied, pvsbc));

        addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    } else
    {
        setIndexStatic();
    }

    addBindable(createScope<TransformConstantBuffer>(gfx, *this));
}

mat4f mage::Sheet::getTransformMatrix() const noexcept
{
    return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
           dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}

void mage::Sheet::update(float delta) noexcept
{
    roll += droll * delta;
    pitch += dpitch * delta;
    yaw += dyaw * delta;
    theta += dtheta * delta;
    phi += dphi * delta;
    chi += dchi * delta;
}
