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
#include "mage/graphics/primitives.h"
#include "mage/graphics/sampler.h"
#include "mage/graphics/texture.h"
#include "mage/graphics/texture_surface.h"

mage::Box::Box(mage::Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
               std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist,
               std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist) :
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
        };
        auto model = Cube::make<Vertex>();
        //model.transform( dx::XMMatrixScaling( 1.0f,1.0f,1.2f ) );

        addStaticBind(createScope<VertexBuffer>(gfx, model.vertices));

        auto pvs = createScope<VertexShader>(gfx, L"basicVS.cso");
        auto pvsbc = pvs->getBytecode();
        addStaticBind(std::move(pvs));

        addStaticBind(createScope<PixelShader>(gfx, L"basicPS.cso"));

        addStaticIndexBuffer(createScope<IndexBuffer>(gfx, model.indices));

        struct ConstantBuffer2 {
            struct {
                float r;
                float g;
                float b;
                float a;
            } face_colors[ 8 ];
        };
        const ConstantBuffer2 cb2 = { {
                { 1.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f },
                { 1.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f },
                { 1.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 1.0f },
                { 0.0f, 0.0f, 0.0f },
        } };
        addStaticBind(createScope<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

        const list<D3D11_INPUT_ELEMENT_DESC> ied = {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        addStaticBind(createScope<InputLayout>(gfx, ied, pvsbc));

        addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    } else
    {
        setIndexStatic();
    }
    addBindable(createScope<TransformConstantBuffer>(gfx, *this));

    dx::XMStoreFloat3x3(&mt, dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
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
    return dx::XMLoadFloat3x3(&mt) * dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
           dx::XMMatrixTranslation(r, 0.0f, 0.0f) * dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
           dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
mat4f mage::SkinnedBox::getTransformMatrix() const noexcept
{
    return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
           dx::XMMatrixRotationRollPitchYaw(theta, phi, chi) * dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
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
