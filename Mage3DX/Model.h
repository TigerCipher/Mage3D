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
 * File Name: Model.h
 * Date File Created: 10/5/2020 at 6:17 PM
 * Author: Matt
 */
#pragma once

#include "ImguiManager.h"
#include "Mesh.h"
#include "ConstantBuffer.h"

class Node
{
friend class Model;
friend class ModelWindow;
public:

	Node(int id, std::string name, list<Mesh*> meshes, const mat4f& transform) noexcept(!MAGE_DEBUG) :
		mMeshes(std::move(meshes)),
		mName(std::move(name)),
		mId(id)
	{
		dx::XMStoreFloat4x4(&mBaseTransform, transform);
		dx::XMStoreFloat4x4(&mAppliedTransform, dx::XMMatrixIdentity());
	}

	void render(Graphics& gfx, mat4f accumulatedTransform) const noexcept(!MAGE_DEBUG);

	void showTree(Node*& selectedNode) const noexcept;

	void setAppliedTransform(mat4f transform) noexcept;

	int getId() const noexcept { return mId; }

	struct MaterialConstFull
	{
		BOOL normalMapEnabled = TRUE;
		BOOL specularMapEnabled = TRUE;
		BOOL hasGlossMap = FALSE;
		float specularPower = 3.1f;
		vec3f specularColor = { 0.75f, 0.75f, 0.75f };
		float specularMapWeight = 0.671f;
	};

	struct MaterialConstNotex
	{
		alignas(16) vec4f materialColor = { 0.447970f,0.327254f,0.176283f, 1.0f };
		alignas(16) vec4f specColor = { 0.65f, 0.65f, 0.65f, 1.0f };
		float specPower = 120.0f;
	};

	template<typename T>
	bool showMaterialControl(Graphics& gfx, T& c)
	{
		if (mMeshes.empty()) return false;

		if constexpr (std::is_same<T, MaterialConstFull>::value)
		{
			if (auto* cb = mMeshes.front()->queryBindable<PixelConstantBuffer<T> >())
			{
				IMGUI_FUNC(Text("Material"));

				bool normMap = static_cast<bool>(c.normalMapEnabled);
				IMGUI_FUNC(Checkbox("Norm. Map", &normMap));
				c.normalMapEnabled = normMap ? TRUE : FALSE;

				bool specMap = static_cast<bool>(c.specularMapEnabled);
				IMGUI_FUNC(Checkbox("Spec. Map", &specMap));
				c.specularMapEnabled = specMap ? TRUE : FALSE;

				bool glossMap = static_cast<bool>(c.hasGlossMap);
				IMGUI_FUNC(Checkbox("Gloss Map", &glossMap));
				c.hasGlossMap = glossMap ? TRUE : FALSE;

				IMGUI_FUNC(SliderFloat("Spec. Weight", &c.specularMapWeight, 0.0f, 2.0f));
				IMGUI_FUNC(SliderFloat("Spec. Pow.", &c.specularPower, 0.0f, 1000.0f, "%f", 5.0f));
				IMGUI_FUNC(ColorPicker3("Spec. Color", reinterpret_cast<float*>(&c.specularColor)));

				cb->update(gfx, c);
				return true;
			}
		}else if constexpr (std::is_same<T, MaterialConstNotex>::value)
		{
			if (auto* cb = mMeshes.front()->queryBindable<PixelConstantBuffer<T> >())
			{
				IMGUI_FUNC(Text("Material"));
				IMGUI_FUNC(ColorPicker3("Spec. Color.", reinterpret_cast<float*>(&c.specColor)));
				IMGUI_FUNC(SliderFloat("Spec. Pow.", &c.specPower, 0.0f, 1000.0f, "%f", 5.0f));
				IMGUI_FUNC(ColorPicker3("Dif. Color", reinterpret_cast<float*>(&c.materialColor)));
				cb->update(gfx, c);
				return true;
			}
		}

		return false;
	}

private:

	void addChild(UniquePtr<Node> child) noexcept(!MAGE_DEBUG);

	list<UniquePtr<Node> > mChildren;
	list<Mesh*> mMeshes;

	mat4x4 mBaseTransform;
	mat4x4 mAppliedTransform;

	std::string mName;
	int mId;
};

class Model
{
public:
	Model(Graphics& gfx, const std::string& fileName, const float scale = 1.0f);
	virtual ~Model() noexcept;

	UniquePtr<Node> parseNode(int& nextId, const struct aiNode& node) noexcept;


	void render(Graphics& gfx) const noexcept(!MAGE_DEBUG);

	void setRootTransform(mat4f tf) const noexcept;

	void showImguiWindow(Graphics& gfx, const char* windowName) const noexcept;


private:
	static UniquePtr<Mesh> parseMesh(Graphics& gfx, const struct aiMesh& mesh, const struct
	                                 aiMaterial* const* materials, const float scale);
	
	UniquePtr<Node> mRoot;
	list<UniquePtr<Mesh> > mMeshes;

	UniquePtr<class ModelWindow> mWindow;
};

