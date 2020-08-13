/*
 * Mage3D
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
 * File Name: model.cpp
 * Date File Created: 8/4/2020 at 7:16 PM
 * Author: Matt
 */

#include "mage/graphics/model.h"
#include <iostream>


mage::Model::Model(const char* path) :
		m_fileName(path)
{
	loadModel(path);
}

void mage::Model::loadModel(const char* path)
{
	DBGPRINT("Loading model %s", path);
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate/* | aiProcess_FlipUVs*/);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		DBGPRINT_ERR("Error while loading model %s -> %s", path, importer.GetErrorString());
		return;
	}
	processNode(scene->mRootNode, scene);
}

void mage::Model::processNode(aiNode* node, const aiScene* scene)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[ node->mMeshes[ i ] ];
		m_meshes.push_back(processMesh(mesh, scene));
	}

	// Rinse and repeat for any child nodes
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[ i ], scene);
	}
}

mage::Mesh* mage::Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	list<Vertex> vertices;
	list<uint> indices;
	//list<Texture> textures;

	// Fill out Vertex struct
	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		Position pos(mesh->mVertices[ i ].x, mesh->mVertices[ i ].y, mesh->mVertices[ i ].z);
		Normal normal(mesh->mNormals[ i ].x, mesh->mNormals[ i ].y, mesh->mNormals[ i ].z);
		Vertex vert(pos, normal);
		if (mesh->mTextureCoords[ 0 ])
			vert.setTexCoords(mesh->mTextureCoords[ 0 ][ i ].x, mesh->mTextureCoords[ 0 ][ i ].y);
		vertices.push_back(vert);
	}

	// Get the indices for the mesh
	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[ i ];
		for (uint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[ j ]);
		}
	}

	// Get the material/textures
	//if (mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial* mat = scene->mMaterials[ mesh->mMaterialIndex ];
	//	list<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE);
	//	for (const auto& item : diffuseMaps)
	//	{
	//		textures.push_back(item);
	//	}
	//	list<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, TEXTURE_SPECULAR);
	//	for (const auto& item : specularMaps)
	//	{
	//		textures.push_back(item);
	//	}
	//}
	return new Mesh(vertices, indices);
}

#include <iostream>

list<mage::Texture>
mage::Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName)
{
	list<Texture> textures;
	for (uint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string file("./assets/textures/");
		file.append(str.C_Str());
		Texture tex(file.c_str(), typeName);
		textures.push_back(tex);
	}
	return textures;
}

mage::Model::~Model()
{
	DBGPRINT("Unloading model %s", m_fileName.c_str());
	for (auto& mesh : m_meshes)
	{
		// if(mesh) delete mesh; -> according to clang-tidy deleting null ptr has no effect unlike free?
		delete mesh;
	}
}


