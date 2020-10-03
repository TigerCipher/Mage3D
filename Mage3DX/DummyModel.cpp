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
 * File Name: DummyModel.cpp
 * Date File Created: 10/2/2020 at 4:06 PM
 * Author: Matt
 */
#include "pch.h"
#include "DummyModel.h"
#include "Bindables.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

mage::DummyModel::DummyModel(Graphics& gfx, std::mt19937& rng,
                             std::uniform_real_distribution<float>& adist,
                             std::uniform_real_distribution<float>& ddist,
                             std::uniform_real_distribution<float>& odist,
                             std::uniform_real_distribution<float>& rdist,
                             DirectX::XMFLOAT3 material, float scale) :
	DummyObject(gfx, rng, adist, ddist, odist, rdist)
{
	if (!isInitialized())
	{
		struct Vertex
		{
			vec3f pos;
			vec3f normal;
		};

		Assimp::Importer imp;
		const auto model = imp.ReadFile("assets\\models\\suzanne.obj", aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices);
		const auto mesh = model->mMeshes[0];

		list<Vertex> vertices;
		vertices.reserve(mesh->mNumVertices);
		for (uint i = 0; i < mesh->mNumVertices; i++)
		{
			vertices.push_back({ { mesh->mVertices[i].x * scale, mesh->mVertices[i].y * scale,
					mesh->mVertices[i].z * scale },
				    *reinterpret_cast<vec3f*>(&mesh->mNormals[i]) });
		}

		list<ushort> indices;
		indices.reserve(mesh->mNumFaces * 3);
		for (ushort i = 0; i < mesh->mNumFaces; i++)
		{
			const auto& face = mesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		addStaticBind(createScope<VertexBuffer>(gfx, vertices));
		addStaticIndexBuffer(createScope<IndexBuffer>(gfx, indices));

		auto pvs = createScope<VertexShader>(gfx, L"shaders\\phongVS.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(createScope<PixelShader>(gfx, L"shaders\\phongPS.cso"));

		const list<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		addStaticBind(createScope<InputLayout>(gfx, ied, pvsbc));
		addStaticBind(createScope<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


		struct MaterialConst
		{
			vec3f color;
			float intensity = 0.6f;
			float power = 30.0f;
			float padding[3];
		} matConst;

		matConst.color = material;
		addStaticBind(createScope<PixelConstantBuffer<MaterialConst>>(gfx, matConst, 1));

	}
	else
	{
		setIndexStatic();
	}

	addBind(createScope<TransformConstantBuffer>(gfx, *this));
}