/*
 * Mage3DX
 * Copyright (C) 2021 Blue Moon Development. All rights reserved.
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
 * File Name: TextureProcessor.cpp
 * Date File Created: 1/26/2021 at 11:25 PM
 * Author: Matt
 */
#include "TextureProcessor.h"
#include "ModelException.h"

#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>


void TextureProcessor::rotateXAxis(const std::string& pathSrc, const std::string& pathDest)
{
	const auto rot = rotateXMatrix(PI);

	const auto func = [rot] (const vec n) -> vec
		{
			return transformVector3(n, rot);
		};

	transformFile(pathSrc, pathDest, func);
}

void TextureProcessor::flipYForAllModelNormalMaps(const std::string& modelPath)
{
	using namespace std::string_literals;
	const auto texturePath = "assets\\textures\\"s;

	Assimp::Importer imp;
	const auto* scene = imp.ReadFile(modelPath.c_str(), 0);
	if(!scene)
	{
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
	}

	const auto flipY = setVector(1, -1, 1, 1);
	const auto func = [flipY] (const vec n) -> vec
		{
			return mulVector(n, flipY);
		};


	for(uint i = 0; i < scene->mNumMaterials; i++)
	{
		const auto& mat = *scene->mMaterials[i];
		aiString texFile;
		if(mat.GetTexture(aiTextureType_NORMALS, 0, &texFile) == aiReturn_SUCCESS)
		{
			const auto path = texturePath + texFile.C_Str();
			const auto backupPath = std::filesystem::path(path).parent_path().string() + "\\backup"s;
			std::filesystem::create_directories(backupPath);
			std::filesystem::copy_file(path, backupPath + path.substr(path.find_last_of('\\')),
				std::filesystem::copy_options::overwrite_existing);
			transformFile(path, path, func);
		}
	}
}


vec TextureProcessor::colorToVector(const Color& col)
{
	auto n = setVector(static_cast<float>(col.getRed()),
		static_cast<float>(col.getGreen()),
		static_cast<float>(col.getBlue()),
		0.0f);

	const auto all255 = replicateVector(2.0f / 255.0f);
	n = mulVector(n, all255);
	const auto all1 = replicateVector(1.0f);
	n = subVector(n, all1);
	return n;
}

Color TextureProcessor::vectorToColor(const vec n)
{
	const auto all1 = replicateVector(1.0f);
	vec out = addVector(n, all1);
	const auto all255 = replicateVector(255.0f / 2.0f);
	out = mulVector(out, all255);
	vec3f floats;
	storeVector3(&floats, out);
	return {
	    static_cast<uchar>(round(floats.x)),
	    static_cast<uchar>(round(floats.y)),
	    static_cast<uchar>(round(floats.z))
	};
}
