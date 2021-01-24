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
#include "ImguiManager.h"
#include "ModelException.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "TextureException.h"


void Node::render(Graphics& gfx, mat4f accumulatedTransform) const noexcept(!MAGE_DEBUG)
{
	const auto transform = dx::XMLoadFloat4x4(&mBaseTransform)
	    * dx::XMLoadFloat4x4(&mAppliedTransform)
	    * accumulatedTransform;

	for (const auto* m : mMeshes)
	{
		m->render(gfx, transform);
	}

	for(const auto& c : mChildren)
	{
		c->render(gfx, transform);
	}
}

void Node::showTree(Node*& selectedNode) const noexcept
{
	const int selectedId = !selectedNode ? -1 : selectedNode->getId();
	const auto nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
	    | ImGuiTreeNodeFlags_OpenOnDoubleClick
	    | ((mId == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
	    | ((mChildren.empty()) ? ImGuiTreeNodeFlags_Leaf : 0);


	const auto expanded = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<intptr_t>(mId)),
		nodeFlags, mName.c_str());

	if (ImGui::IsItemClicked())
	{
		selectedNode = const_cast<Node*>(this);
	}


	if(expanded)
	{
		for(const auto& child : mChildren)
		{
			child->showTree(selectedNode);
		}
		ImGui::TreePop();
	}
}

void Node::setAppliedTransform(mat4f transform) noexcept
{
	dx::XMStoreFloat4x4(&mAppliedTransform, transform);
}

void Node::addChild(UniquePtr<Node> child) noexcept(!MAGE_DEBUG)
{
	assert(child && "Child must not be null");
	mChildren.push_back(std::move(child));
}



//////////// Internal imgui window class //////////////////////
class ModelWindow
{
public:
	void show(const char* windowName, const Node & root) noexcept
	{
		if (!windowName) windowName = "Model";

		int nodeIndex = 0;

		IMGUI_BEGIN(windowName)
		IMGUI_FUNC(Columns(2, nullptr, true));
		root.showTree(mSelectedNode);

		IMGUI_FUNC(NextColumn());

		if(mSelectedNode)
		{
			auto& transform = mTransforms[mSelectedNode->getId()];
			IMGUI_FUNC(Text("Node ID: %i", mSelectedNode->getId()));
			IMGUI_FUNC(Text("Orientation"));
			IMGUI_FUNC(SliderAngle("Roll", &transform.roll, -180, 180));
			IMGUI_FUNC(SliderAngle("Pitch", &transform.pitch, -180, 180));
			IMGUI_FUNC(SliderAngle("Yaw", &transform.yaw, -180, 180));
			IMGUI_FUNC(Text("Position"));
			IMGUI_FUNC(SliderFloat("X", &transform.x, -20, 20));
			IMGUI_FUNC(SliderFloat("Y", &transform.y, -20, 20));
			IMGUI_FUNC(SliderFloat("Z", &transform.z, -20, 20));
		}
		IMGUI_END
	}

	[[nodiscard]] mat4f getTransform() const noexcept
	{
		const auto& transform = mTransforms.at(mSelectedNode->getId());
		return dx::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw)
		       * dx::XMMatrixTranslation(transform.x, transform.y, transform.z);
	}

	[[nodiscard]] Node* getSelectedNode() const noexcept
	{
		return mSelectedNode;
	}
private:
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};


	Node* mSelectedNode;
	std::unordered_map<int, TransformParameters> mTransforms;
};


//////////////////////////////////////////////////////////////

Model::Model(Graphics& gfx, const std::string& fileName) :
	mWindow(createScope<ModelWindow>())
{
	Assimp::Importer imp;

	const aiScene* scene = imp.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
		| aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if(!scene)
	{
		std::string errStr = imp.GetErrorString();
		LOG_ERROR("Failed to load model {} with error reported by Assimp: {}", fileName, errStr);
		MODEL_THROW_EXCEPTION(errStr);
	}

	for(size_t i = 0; i < scene->mNumMeshes; i++)
	{
		mMeshes.push_back(parseMesh(gfx, *scene->mMeshes[i], scene->mMaterials));
	}

	int nextId = 0;
	mRoot = parseNode(nextId, *scene->mRootNode);
}

Model::~Model() noexcept = default;

UniquePtr<Node> Model::parseNode(int& nextId, const aiNode& node) noexcept
{
	const auto transform = dx::XMMatrixTranspose(
		dx::XMLoadFloat4x4(reinterpret_cast<const mat4x4*>(&node.mTransformation)));

	list<Mesh*> currentMeshes;
	currentMeshes.reserve(node.mNumMeshes);

	for(size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshId = node.mMeshes[i];
		currentMeshes.push_back(mMeshes.at(meshId).get());
	}

	auto currentNode = createScope<Node>(nextId++, node.mName.C_Str(), std::move(currentMeshes), transform);
	for(size_t i = 0; i < node.mNumChildren; i++)
	{
		currentNode->addChild(parseNode(nextId, *node.mChildren[i]));
	}

	return currentNode;
}

UniquePtr<Mesh> Model::parseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* materials)
{
	bool hasSpecMap = false;
	float shininess = 35.0f;

	VertexBuffer vData(std::move(VertexLayout{ }.append(POSITION3D).append(NORMAL)
		.append(TANGENT).append(BITANGENT).append(TEXTURE2D)));

	for (uint i = 0; i < mesh.mNumVertices; i++)
	{
		vData.emplaceBack(
			*reinterpret_cast<vec3f*>(&mesh.mVertices[i]),
			*reinterpret_cast<vec3f*>(&mesh.mNormals[i]),
			*reinterpret_cast<vec3f*>(&mesh.mTangents[i]),
			*reinterpret_cast<vec3f*>(&mesh.mBitangents[i]),
			*reinterpret_cast<vec2f*>(&mesh.mTextureCoords[0][i])
			);
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

	list<SharedPtr<Bindable> > binds;

	using namespace std::string_literals;
	const auto basePath = "assets\\textures\\"s;

	if(mesh.mMaterialIndex >= 0)
	{
		const auto& material = *materials[mesh.mMaterialIndex];
		aiString textureFile;

		material.GetTexture(aiTextureType_DIFFUSE, 0, &textureFile);
		binds.push_back(TextureData::resolve(gfx, basePath + textureFile.C_Str()));

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &textureFile) == aiReturn_SUCCESS)
		{
			hasSpecMap = true;
			binds.push_back(TextureData::resolve(gfx, basePath + textureFile.C_Str(), 1));
		}
		else
		{
			material.Get(AI_MATKEY_SHININESS, shininess);
		}

		if(material.GetTexture(aiTextureType_NORMALS, 0, &textureFile) == aiReturn_SUCCESS)
		{
			binds.push_back(TextureData::resolve(gfx, basePath + textureFile.C_Str(), 2));
		}

		binds.push_back(Sampler::resolve(gfx));
	}

	auto meshTag = basePath + "%" + mesh.mName.C_Str();

	binds.push_back(VertexBufferBindable::resolve(gfx, meshTag, vData));
	binds.push_back(IndexBuffer::resolve(gfx, meshTag, indices));

	auto pvs = VertexShader::resolve(gfx, "shaders\\phongNormalVS.cso");
	auto* pvsbc = pvs->getBytecode();
	binds.push_back(std::move(pvs));

	binds.push_back(InputLayout::resolve(gfx, vData.getLayout(), pvsbc));

	if(hasSpecMap)
	{
		binds.push_back(PixelShader::resolve(gfx, "shaders\\phongSpecPS.cso"));
		struct MaterialConst
		{
			BOOL normalMapEnabled = TRUE;
			float padding[3];
		} matConst;
		binds.push_back(PixelConstantBuffer<MaterialConst>::resolve(gfx, matConst, 1));
	}
	else
	{
		binds.push_back(PixelShader::resolve(gfx, "shaders\\phongNormalPS.cso"));
		struct MaterialConst
		{
			float specIntensity = 0.18f;
			float specPower = 20.0f;
			BOOL normalMapEnabled = TRUE;
			float padding[1];
		} matConst;
		matConst.specPower = shininess;

		// TODO: Meshes will end up sharing the same material
		binds.push_back(PixelConstantBuffer<MaterialConst>::resolve(gfx, matConst, 1));
	}

	return createScope<Mesh>(gfx, std::move(binds));
}

void Model::render(Graphics& gfx) const noexcept(!MAGE_DEBUG)
{
	if(auto node = mWindow->getSelectedNode())
	{
		node->setAppliedTransform(mWindow->getTransform());
	}
	mRoot->render(gfx, dx::XMMatrixIdentity());
}

void Model::setRootTransform(const mat4f tf) const noexcept
{
	mRoot->setAppliedTransform(tf);
}

void Model::showImguiWindow(const char* windowName) const noexcept
{
	mWindow->show(windowName, *mRoot);
}



