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
// ReSharper disable CppClangTidyClangDiagnosticImplicitIntConversion
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
	void show(Graphics& gfx, const char* windowName, const Node & root) noexcept
	{
		if (!windowName) windowName = "Model";

		int nodeIndex = 0;
		IMGUI_BEGIN(windowName)
		{
			IMGUI_FUNC(Columns(2, nullptr, true));
			root.showTree(mSelectedNode);

			IMGUI_FUNC(NextColumn());

			if(mSelectedNode)
			{
				const auto id = mSelectedNode->getId();
				auto i = mTransforms.find(id);
				if(i == mTransforms.end())
				{
					const auto& applied = mSelectedNode->getAppliedTransform();
					const auto angles = extract_euler_angles(applied);
					const auto translation = extract_translation(applied);

					TransformParameters tp = {
						angles.z, angles.x, angles.y,
						translation.x, translation.y, translation.z
					};

					std::tie(i, std::ignore) = mTransforms.insert({ id, tp });
				}
				auto& transform = i->second;
				
				IMGUI_FUNC(Text("Node ID: %i", mSelectedNode->getId()));
				IMGUI_FUNC(Text("Orientation"));
				IMGUI_FUNC(SliderAngle("Roll", &transform.roll, -180, 180));
				IMGUI_FUNC(SliderAngle("Pitch", &transform.pitch, -180, 180));
				IMGUI_FUNC(SliderAngle("Yaw", &transform.yaw, -180, 180));
				IMGUI_FUNC(Text("Position"));
				IMGUI_FUNC(SliderFloat("X", &transform.x, -200, 200));
				IMGUI_FUNC(SliderFloat("Y", &transform.y, -200, 200));
				IMGUI_FUNC(SliderFloat("Z", &transform.z, -200, 200));

				// Material
				if(!mSelectedNode->showMaterialControl(gfx, mMatFull))
				{
					mSelectedNode->showMaterialControl(gfx, mMatNotex);
				}
			}
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
	Node::MaterialConstFull mMatFull;
	Node::MaterialConstNotex mMatNotex;
	std::unordered_map<int, TransformParameters> mTransforms;
};


//////////////////////////////////////////////////////////////

Model::Model(Graphics& gfx, const std::string& fileName, const float scale) :
	mWindow(createScope<ModelWindow>())
{
	LOG_TRACE("Loading model [{}]", fileName);
	Assimp::Importer imp;

	const auto* scene = imp.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
	                          | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if(!scene)
	{
		const std::string errStr = imp.GetErrorString();
		LOG_ERROR("Failed to load model {} with error reported by Assimp: {}", fileName, errStr);
		MODEL_THROW_EXCEPTION(errStr);
	}
	mNumMeshes = static_cast<float>(scene->mNumMeshes);
	for(size_t i = 0; i < scene->mNumMeshes; i++)
	{
		mMeshes.push_back(parseMesh(gfx, *scene->mMeshes[i], scene->mMaterials, scale));
	}

	int nextId = 0;
	mRoot = parseNode(nextId, *scene->mRootNode);

	LOG_INFO("Loaded model [{}]", fileName);
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


UniquePtr<Mesh> Model::parseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* materials, const float scale)
{
	mMeshesLoaded++;
	LOG_DEBUG("Loading mesh {} - {:.2f}%", mesh.mName.C_Str(), (mMeshesLoaded / mNumMeshes) * 100.0f);
	bool hasSpecMap = false;
	bool hasNormalMap = false;
	bool hasDiffuseMap = false;
	bool hasAlphaGloss = false;
	float shininess = 2.0f;
	vec4f specColor = { 0.18f, 0.18f, 0.18f, 1.0f };
	vec4f diffColor = { 0.85f, 0.45f, 0.85f, 1.0f };

	list<SharedPtr<Bindable> > binds;

	using namespace std::string_literals;
	const auto basePath = "assets\\textures\\"s;

	if(mesh.mMaterialIndex >= 0)
	{
		const auto& material = *materials[mesh.mMaterialIndex];
		aiString textureFile;

		if(material.GetTexture(aiTextureType_DIFFUSE, 0, &textureFile) == aiReturn_SUCCESS)
		{
			binds.push_back(TextureData::resolve(gfx, basePath + textureFile.C_Str()));
			hasDiffuseMap = true;
		}else
		{
			material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffColor));
		}

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &textureFile) == aiReturn_SUCCESS)
		{
			auto spec = TextureData::resolve(gfx, basePath + textureFile.C_Str(), 1);
			hasAlphaGloss = spec->hasAlpha();
			hasSpecMap = true;
			binds.push_back(std::move(spec));
		}else
		{
			material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specColor));
		}
		if(!hasAlphaGloss)
		{
			material.Get(AI_MATKEY_SHININESS, shininess);
		}

		if(material.GetTexture(aiTextureType_NORMALS, 0, &textureFile) == aiReturn_SUCCESS)
		{
			auto norm = TextureData::resolve(gfx, basePath + textureFile.C_Str(), 2);
			hasNormalMap = true;
			hasAlphaGloss = norm->hasAlpha();
			binds.push_back(std::move(norm));
		}

		if(hasDiffuseMap || hasSpecMap || hasNormalMap)
			binds.push_back(Sampler::resolve(gfx));
	}

	const auto meshTag = basePath + "%" + mesh.mName.C_Str();

	VertexBuffer vData(std::move(VertexLayout{ }.append(POSITION3D).append(NORMAL)));
	std::string vertShader;
	std::string pixShader;

	
	if(hasDiffuseMap && hasNormalMap && hasSpecMap)
	{
		
		 vData = VertexBuffer(std::move(VertexLayout{ }.append(POSITION3D).append(NORMAL)
			.append(TANGENT).append(BITANGENT).append(TEXTURE2D)));

		for (uint i = 0; i < mesh.mNumVertices; i++)
		{
			vData.emplaceBack(
				vec3f(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<vec3f*>(&mesh.mNormals[i]),
				*reinterpret_cast<vec3f*>(&mesh.mTangents[i]),
				*reinterpret_cast<vec3f*>(&mesh.mBitangents[i]),
				*reinterpret_cast<vec2f*>(&mesh.mTextureCoords[0][i])
			);
		}

		vertShader = "shaders\\phongNormalVS.cso";
		pixShader = "shaders\\phongSpecNormalPS.cso";

		Node::MaterialConstFull matConst;
		matConst.specularPower = shininess;
		matConst.hasGlossMap = hasAlphaGloss ? TRUE : FALSE;
		binds.push_back(PixelConstantBuffer<Node::MaterialConstFull>::resolve(gfx, matConst, 1));
		
	} else if(hasDiffuseMap && hasNormalMap)
	{
		vData = VertexBuffer(std::move(VertexLayout{ }.append(POSITION3D).append(NORMAL)
			.append(TANGENT).append(BITANGENT).append(TEXTURE2D)));

		for (uint i = 0; i < mesh.mNumVertices; i++)
		{
			vData.emplaceBack(
				vec3f(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<vec3f*>(&mesh.mNormals[i]),
				*reinterpret_cast<vec3f*>(&mesh.mTangents[i]),
				*reinterpret_cast<vec3f*>(&mesh.mBitangents[i]),
				*reinterpret_cast<vec2f*>(&mesh.mTextureCoords[0][i])
			);
		}

		list<ushort> indices;
		indices.reserve(mesh.mNumFaces * 3);

		for (uint i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		vertShader = "shaders\\phongNormalVS.cso";
		pixShader = "shaders\\phongNormalPS.cso";
		struct MaterialConst
		{
			float specIntensity{};
			float specPower{};
			BOOL normalMapEnabled = TRUE;
			float padding[1]{};
		} matConst;
		matConst.specPower = shininess;
		matConst.specIntensity = (specColor.x + specColor.y + specColor.z) / 3.0f;
		binds.push_back(PixelConstantBuffer<MaterialConst>::resolve(gfx, matConst, 1));
	}else if(hasDiffuseMap)
	{
		vData = VertexBuffer(std::move(VertexLayout{ }.append(POSITION3D).append(NORMAL)
			.append(TEXTURE2D)));

		for (uint i = 0; i < mesh.mNumVertices; i++)
		{
			vData.emplaceBack(
				vec3f(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<vec3f*>(&mesh.mNormals[i]),
				*reinterpret_cast<vec2f*>(&mesh.mTextureCoords[0][i])
			);
		}

		vertShader = "shaders\\phongVS.cso";
		pixShader = "shaders\\phongPS.cso";
		
		struct MaterialConst
		{
			float specIntensity;
			float specPower;
			float padding[2];
		} matConst;
		matConst.specPower = shininess;
		matConst.specIntensity = (specColor.x + specColor.y + specColor.z) / 3.0f;
		binds.push_back(PixelConstantBuffer<MaterialConst>::resolve(gfx, matConst, 1));
	}else if(!hasDiffuseMap && !hasNormalMap && !hasSpecMap)
	{
		for (uint i = 0; i < mesh.mNumVertices; i++)
		{
			vData.emplaceBack(
				vec3f(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<vec3f*>(&mesh.mNormals[i])
			);
		}

		vertShader = "shaders\\phongNotexVS.cso";
		pixShader = "shaders\\phongNotexPS.cso";

		Node::MaterialConstNotex matConst;
		matConst.specPower = shininess;
		matConst.specColor = specColor;
		matConst.materialColor = diffColor; 
		binds.push_back(PixelConstantBuffer<Node::MaterialConstNotex>::resolve(gfx, matConst, 1));
	}
	else if (hasDiffuseMap && hasSpecMap && !hasNormalMap)
	{
		vData = VertexBuffer(std::move(VertexLayout{ }.append(POSITION3D).append(NORMAL)
			.append(TEXTURE2D)));

		for (uint i = 0; i < mesh.mNumVertices; i++)
		{
			vData.emplaceBack(
				vec3f(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<vec3f*>(&mesh.mNormals[i]),
				*reinterpret_cast<vec2f*>(&mesh.mTextureCoords[0][i])
			);
		}

		vertShader = "shaders\\phongVS.cso";
		pixShader = "shaders\\phongSpecPS.cso";

		struct MaterialConstant
		{
			float specularPower;
			BOOL hasGloss;
			float specularMapWeight;
			float padding;
		}matConst;

		matConst.specularPower = shininess;
		matConst.hasGloss = hasAlphaGloss ? TRUE : FALSE;
		matConst.specularMapWeight = 1.0f;
		binds.push_back(PixelConstantBuffer<MaterialConstant>::resolve(gfx, matConst, 1));
	}else
	{
		throw ModelException(__LINE__, __FILE__, "Failed to parse mesh");
	}


	list<ushort> indices;
	indices.reserve(mesh.mNumFaces * 3);

	for (uint i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	binds.push_back(VertexBufferBindable::resolve(gfx, meshTag, vData));
	binds.push_back(IndexBuffer::resolve(gfx, meshTag, indices));
	auto pvs = VertexShader::resolve(gfx, vertShader);
	auto* pvsbc = pvs->getBytecode();
	binds.push_back(std::move(pvs));

	binds.push_back(InputLayout::resolve(gfx, vData.getLayout(), pvsbc));
	binds.push_back(PixelShader::resolve(gfx, pixShader));
	

	return createScope<Mesh>(gfx, std::move(binds));
}

void Model::render(Graphics& gfx) const noexcept(!MAGE_DEBUG)
{
	if(auto* node = mWindow->getSelectedNode())
	{
		node->setAppliedTransform(mWindow->getTransform());
	}
	mRoot->render(gfx, dx::XMMatrixIdentity());
}

void Model::setRootTransform(const mat4f tf) const noexcept
{
	mRoot->setAppliedTransform(tf);
}

void Model::showImguiWindow(Graphics& gfx, const char* windowName) const noexcept
{
	mWindow->show(gfx, windowName, *mRoot);
}



