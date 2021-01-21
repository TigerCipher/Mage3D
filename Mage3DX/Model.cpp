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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



void Node::render(Graphics& gfx, mat4f accumulatedTransform) const noexcept(!MAGE_DEBUG)
{
	const auto transform = dx::XMLoadFloat4x4(&mAppliedTransform)
	    * dx::XMLoadFloat4x4(&mBaseTransform)
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

void Node::showTree(int& index, std::optional<int>& selectedIndex, Node*& selectedNode) const noexcept
{
	const int currentIndex = index;
	index++;
	const auto nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
	    | ImGuiTreeNodeFlags_OpenOnDoubleClick
	    | ((currentIndex == selectedIndex.value_or(-1)) ? ImGuiTreeNodeFlags_Selected : 0)
	    | ((mChildren.empty()) ? ImGuiTreeNodeFlags_Leaf : 0);


	const auto expanded = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<intptr_t>(currentIndex)),
		nodeFlags, mName.c_str());

	if (ImGui::IsItemClicked())
	{
		selectedIndex = currentIndex;
		selectedNode = const_cast<Node*>(this);
	}


	if(expanded)
	{
		for(const auto& child : mChildren)
		{
			child->showTree(index, selectedIndex, selectedNode);
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

		if (ImGui::Begin(windowName)) {
			ImGui::Columns(2, nullptr, true);
			root.showTree(nodeIndex, mSelectedIndex, mSelectedNode);

			ImGui::NextColumn();

			if(mSelectedNode)
			{
				auto& transform = mTransforms[*mSelectedIndex];
				ImGui::Text("Orientation");
				ImGui::SliderAngle("Roll", &transform.roll, -180, 180);
				ImGui::SliderAngle("Pitch", &transform.pitch, -180, 180);
				ImGui::SliderAngle("Yaw", &transform.yaw, -180, 180);
				ImGui::Text("Position");
				ImGui::SliderFloat("X", &transform.x, -20, 20);
				ImGui::SliderFloat("Y", &transform.y, -20, 20);
				ImGui::SliderFloat("Z", &transform.z, -20, 20);
			}
		}
		ImGui::End();
	}

	mat4f getTransform() const noexcept
	{
		const auto& transform = mTransforms.at(*mSelectedIndex);
		return dx::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw)
		       * dx::XMMatrixTranslation(transform.x, transform.y, transform.z);
	}

	Node* getSelectedNode() const noexcept
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


	std::optional<int> mSelectedIndex;
	Node* mSelectedNode;
	std::unordered_map<int, TransformParameters> mTransforms;
};


//////////////////////////////////////////////////////////////

Model::Model(Graphics& gfx, const std::string& fileName) :
	mWindow(createScope<ModelWindow>())
{
	Assimp::Importer imp;

	const aiScene* scene = imp.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
		| aiProcess_ConvertToLeftHanded | aiProcess_GenNormals);

	if(!scene)
	{
		std::string errStr = imp.GetErrorString();
		LOG_ERROR("Failed to load model {} with error reported by Assimp: {}", fileName, errStr);
		MODEL_THROW_EXCEPTION(errStr);
	}

	for(size_t i = 0; i < scene->mNumMeshes; i++)
	{
		mMeshes.push_back(parseMesh(gfx, *scene->mMeshes[i]));
	}

	mRoot = parseNode(*scene->mRootNode);
}

Model::~Model() noexcept = default;

UniquePtr<Node> Model::parseNode(const aiNode& node) noexcept
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

	auto currentNode = createScope<Node>(node.mName.C_Str(), std::move(currentMeshes), transform);
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

void Model::render(Graphics& gfx) const noexcept(!MAGE_DEBUG)
{
	if(auto node = mWindow->getSelectedNode())
	{
		node->setAppliedTransform(mWindow->getTransform());
	}
	mRoot->render(gfx, dx::XMMatrixIdentity());
}

void Model::showImguiWindow(const char* windowName) noexcept
{
	mWindow->show(windowName, *mRoot);
}



