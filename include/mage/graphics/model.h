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
 * File Name: model.h
 * Date File Created: 8/4/2020 at 7:16 PM
 * Author: Matt
 */

#ifndef MAGE3D_MODEL_H
#define MAGE3D_MODEL_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "mesh.h"
#include "material.h"

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace mage
{
	class Model
	{
	public:
		mage3d_EXPORT explicit Model(const char* path, const Material* mat = nullptr);

		mage3d_EXPORT virtual ~Model();

		mage3d_EXPORT inline list<Mesh*> getMeshes() { return m_meshes; }
		[[nodiscard]] mage3d_EXPORT inline const Material* getMaterial() { return m_material; }
		[[nodiscard]] mage3d_EXPORT inline const Material* getMaterial() const { return m_material; }
		//list<Material> getMaterials() { return m_materials; }

	private:

		void loadModel(const char* path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
		list<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName);

		list<Mesh*> m_meshes;
		std::string m_fileName;
		const Material* m_material;
		//list<Material> m_materials;
	};

}


#endif //MAGE3D_MODEL_H
