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
public:

	Node(list<Mesh*> meshes, const mat4f& transform) noexcept(!MAGE_DEBUG) :
		mMeshes(std::move(meshes))
	{
		dx::XMStoreFloat4x4(&this->mTransform, transform);
	}

	void render(Graphics& gfx, mat4f accumulatedTransform) const noexcept(!MAGE_DEBUG);

private:

	void addChild(UniquePtr<Node> child) noexcept(!MAGE_DEBUG);
	
	list<UniquePtr<Node> > mChildren;
	list<Mesh*> mMeshes;
	mat4x4 mTransform;
};

class Model
{
public:
	Model(Graphics& gfx, const std::string& fileName);

	UniquePtr<Node> parseNode(const struct aiNode& node);

	static UniquePtr<Mesh> parseMesh(Graphics& gfx, const struct aiMesh& mesh);

	void render(Graphics& gfx, mat4f transform) const;
	
private:
	UniquePtr<Node> mRoot;
	list<UniquePtr<Mesh>> mMeshes;
};

