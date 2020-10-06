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
 * File Name: Model.cpp
 * Date File Created: 10/5/2020 at 6:17 PM
 * Author: Matt
 */
#include "Model.h"
#include "Vertex.h"
#include "Bindables.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void Node::render(Graphics& gfx, mat4f accumulatedTransform) const noexcept(!MAGE_DEBUG)
{
	const auto transform = dx::XMLoadFloat4x4(&m_transform) * accumulatedTransform;

	for (const auto m : m_meshes)
	{
		m->render(gfx, transform);
	}

	for(const auto& c : m_children)
	{
		c->render(gfx, transform);
	}
}

void Node::addChild(UniquePtr<Node> child) noexcept(!MAGE_DEBUG)
{
	assert(child && "Child must not be null");
	m_children.push_back(std::move(child));
}


Model::Model(Graphics& gfx, const std::string& fileName)
{
	Assimp::Importer imp;
	const auto scene = imp.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	for(size_t i = 0; i < scene->mNumMeshes; i++)
	{
		m_meshes.push_back(parseMesh(gfx, *scene->mMeshes[i]));
	}

	m_root = parseNode(*scene->mRootNode);
}

UniquePtr<Node> Model::parseNode(const aiNode& node)
{
	const auto transform = dx::XMMatrixTranspose(
		dx::XMLoadFloat4x4(reinterpret_cast<const mat4x4*>(&node.mTransformation)));

	list<Mesh*> currentMeshes;
	currentMeshes.reserve(node.mNumMeshes);

	for(size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshId = node.mMeshes[i];
		currentMeshes.push_back(m_meshes.at(meshId).get());
	}

	auto currentNode = createScope<Node>(std::move(currentMeshes), transform);
	for(size_t i = 0; i < node.mNumChildren; i++)
	{
		currentNode->addChild(parseNode(*node.mChildren[i]));
	}

	return currentNode;
}

UniquePtr<Mesh> Model::parseMesh(Graphics& gfx, const aiMesh& mesh)
{
	VertexBuffer vData(std::move(VertexLayout().append(POSITION3D).append(NORMAL)));
	for (uint i = 0; i < mesh.mNumVertices; i++)
	{
		vData.emplaceBack(*reinterpret_cast<vec3f*>(&mesh.mVertices[i]),
			*reinterpret_cast<vec3f*>(&mesh.mNormals[i]));
	}

	list<ushort> indices;
	indices.reserve(mesh.mNumFaces * 3);

	for(uint i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	list<UniquePtr<Bindable> > binds;
	binds.push_back(createScope<VertexBufferBindable>(gfx, vData));
	binds.push_back(createScope<IndexBuffer>(gfx, indices));

	auto pvs = createScope<VertexShader>(gfx, L"shaders\\phongVS.cso");
	auto pvsbc = pvs->getBytecode();
	binds.push_back(std::move(pvs));

	binds.push_back(createScope<PixelShader>(gfx, L"shaders\\phongPS.cso"));
	binds.push_back(createScope<InputLayout>(gfx, vData.getLayout().getD3dLayout(), pvsbc));

	struct MaterialConst
	{
		vec3f color = { 0.6f, 0.6f, 0.8f };
		float specIntensity = 0.6f;
		float specPower = 30.0f;

		float padding[3];
	} matConst;

	binds.push_back(createScope<PixelConstantBuffer<MaterialConst> >(gfx, matConst, 1));

	return createScope<Mesh>(gfx, std::move(binds));
}

void Model::render(Graphics& gfx, mat4f transform) const
{
	m_root->render(gfx, transform);
}
