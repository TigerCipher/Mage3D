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

#include "TextureException.h"


void TextureProcessor::rotateXAxis(const std::string& pathSrc, const std::string& pathDest)
{
	const auto rot = rotateXMatrix(PI);

	const auto func = [rot] (const vec n, int x, int y) -> vec
		{
			return transformVector3(n, rot);
		};

	transformFile(pathSrc, pathDest, func);
}

void TextureProcessor::flipYNormalMap(const std::string& src, const std::string& dest)
{
	const auto flipY = setVector(1, -1, 1, 1);
	const auto func = [flipY](const vec n, int x, int y) -> vec
	{
		return mulVector(n, flipY);
	};

	transformFile(src, dest, func);
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

			flipYNormalMap(path, path);
		}
	}
}

void TextureProcessor::reformatTexture(const std::string& fileName)
{
	const std::filesystem::path path = fileName;
	const auto ext = path.extension().string();
	using namespace std::string_literals;
	const auto backupPath = std::filesystem::path(path).parent_path().string() + "\\backup"s;
	std::filesystem::create_directories(backupPath);
	const auto newPath = backupPath + fileName.substr(fileName.find_last_of('\\'));
	LOG_DEBUG("Backing up texture [{}] to [{}]", fileName, newPath);
	std::filesystem::copy_file(path, newPath,
		std::filesystem::copy_options::overwrite_existing);
	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, fileName.c_str(), _TRUNCATE);

	dx::ScratchImage scratch;
	HRESULT hr;

	if (ext == ".dds")
		hr = dx::LoadFromDDSFile(wideName, dx::DDS_FLAGS_NONE, nullptr, scratch);
	else if (ext == ".png" || ext == ".jpg" || ext == ".bmp" || ext == ".tif" || ext == ".gif")
		hr = dx::LoadFromWICFile(wideName, dx::WIC_FLAGS_NONE, nullptr, scratch);
	else
		throw TextureException(__LINE__, __FILE__,
			fmt::format("Textures of format [{}] are not current supported", ext));

	if (FAILED(hr))
	{
		throw TextureException(__LINE__, __FILE__, fmt::format("Could not load texture [{}]", fileName), hr);
	}

	if (scratch.GetImage(0, 0, 0)->format != Texture::TEXTURE_FORMAT)
	{

		LOG_TRACE("Converting texture [{}] from format {} to {}", fileName,
			scratch.GetImage(0, 0, 0)->format, Texture::TEXTURE_FORMAT);
		dx::ScratchImage converted;
		hr = dx::Convert(*scratch.GetImage(0, 0, 0), Texture::TEXTURE_FORMAT,
			dx::TEX_FILTER_DEFAULT, dx::TEX_THRESHOLD_DEFAULT, converted);
		if (FAILED(hr))
		{
			throw TextureException(__LINE__, __FILE__,
				fmt::format("Failed to convert texture format [{}]", fileName), hr);
		}
		LOG_TRACE("Reformatted texture [{}]", fileName);
		const Texture tex(std::move(converted));
		tex.save(fileName);
	}else
	{
		LOG_TRACE("Skipping texture [{}]", fileName);
	}
}


void TextureProcessor::reformatAllTextures(const std::string& modelPath)
{
	using namespace std::string_literals;
	const auto texturePath = "assets\\textures\\"s;

	Assimp::Importer imp;
	const auto* scene = imp.ReadFile(modelPath.c_str(), 0);
	if (!scene)
	{
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
	}


	for (uint i = 0; i < scene->mNumMaterials; i++)
	{
		const auto& mat = *scene->mMaterials[i];
		aiString texFile;
		if (mat.GetTexture(aiTextureType_DIFFUSE, 0, &texFile) == aiReturn_SUCCESS)
		{
			const auto path = texturePath + texFile.C_Str();
			reformatTexture(path);
		}
		if (mat.GetTexture(aiTextureType_NORMALS, 0, &texFile) == aiReturn_SUCCESS)
		{
			const auto path = texturePath + texFile.C_Str();
			reformatTexture(path);
		}
		if (mat.GetTexture(aiTextureType_SPECULAR, 0, &texFile) == aiReturn_SUCCESS)
		{
			const auto path = texturePath + texFile.C_Str();
			reformatTexture(path);
		}
	}
}

void TextureProcessor::validateNormalMap(const std::string& path, const float minThreshold, const float maxThreshold)
{
	LOG_DEBUG("Validating normal map [{}]. See debug console", path);
	auto sum = dx::XMVectorZero();

	const auto func = [minThreshold, maxThreshold, &sum](dx::FXMVECTOR n, int x, int y) -> vec
	{
		const float len = dx::XMVectorGetX(dx::XMVector3Length(n));
		const float z = dx::XMVectorGetZ(n);
		if(len < minThreshold || len > maxThreshold)
		{
			vec3f v;
			storeVector3(&v, n);
			auto s = fmt::format("Bad normal length: {} at ({}, {}). Normal: ({}, {}, {})\n",
			                     len, x, y, v.x, v.y, v.z);
			OutputDebugString(s.c_str());
		}
		if(z < 0.0f)
		{
			vec3f v;
			storeVector3(&v, n);
			auto s = fmt::format("Bad normal z direction at: ({}, {}). Normal: ({}, {}, {})\n", 
				x, y, v.x, v.y, v.z);
			OutputDebugString(s.c_str());
		}
		sum = addVector(sum, n);
		return n;
	};

	auto tex = Texture::loadFromFile(path);
	transformTexture(tex, func);

	vec2f sumv;
	storeVector2(&sumv, sum);
	auto s = fmt::format("Normal map bias: ({}, {})\n", sumv.x, sumv.y);
	OutputDebugString(s.c_str());
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
