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
 * File Name: texture.h
 * Date File Created: 7/28/2020 at 8:50 PM
 * Author: Matt
 */

#ifndef MAGE3D_TEXTURE_H
#define MAGE3D_TEXTURE_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "mage/core/resourcemanager.h"
#include <GL/glew.h>
#include <string>
#include <ostream>
#include <map>

#define TEXTURE_DIFFUSE "diffuse"
#define TEXTURE_SPECULAR "specular"

namespace mage
{

	class TextureData : public ResourceManager
	{
	public:
		mage3d_EXPORT TextureData(GLenum textureTarget, int width, int height, int numTextures, ubyte** data, bool clamp);
		mage3d_EXPORT virtual ~TextureData();

		mage3d_EXPORT void bind(int textureNum);
		mage3d_EXPORT int getWidth() { return m_width; }
		mage3d_EXPORT int getHeight() { return m_height; }

		friend std::ostream& operator<<(std::ostream& os, const TextureData& data);

	private:
		TextureData(TextureData& other) { }
		void operator=(TextureData& other) { }

		GLenum m_textureTarget;
		uint* m_id;
		int m_numTextures;
		int m_width;
		int m_height;
	};

	class Texture
	{
	public:
		mage3d_EXPORT Texture();
		mage3d_EXPORT Texture(const char* filePath);
		mage3d_EXPORT Texture(const char* filePath, const char* type);
		mage3d_EXPORT ~Texture();

		mage3d_EXPORT void enable(uint slot);
		mage3d_EXPORT void disable(uint slot);

		mage3d_EXPORT const std::string getType() { return m_type; }

		friend std::ostream& operator<<(std::ostream& os, const Texture& texture);

	protected:
	private:

		void load(const char* filePath);
		const std::string m_filePath;
		const std::string m_type;

		TextureData* m_textureData;

		static std::map<std::string, TextureData*> textureMap;
	};

}


#endif //MAGE3D_TEXTURE_H
