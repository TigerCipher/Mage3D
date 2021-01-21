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

#include "Mesh.h"

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
	Model(Graphics& gfx, const std::string& fileName);
	virtual ~Model() noexcept;

	UniquePtr<Node> parseNode(int& nextId, const struct aiNode& node) noexcept;

	static UniquePtr<Mesh> parseMesh(Graphics& gfx, const struct aiMesh& mesh);

	void render(Graphics& gfx) const noexcept(!MAGE_DEBUG);

	void showImguiWindow(const char* windowName) noexcept;


private:
	UniquePtr<Node> mRoot;
	list<UniquePtr<Mesh> > mMeshes;

	UniquePtr<class ModelWindow> mWindow;
};

