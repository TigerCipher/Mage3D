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
 * File Name: texture.cpp
 * Date File Created: 7/28/2020 at 8:50 PM
 * Author: Matt
 */

#include "mage/graphics/texture.h"
#include "mage/core/stdcolor.h"
#include "mage/errors/textureexception.h"
#include <SOIL/SOIL.h>
#include <cassert>

std::map<std::string, mage::TextureData*> mage::Texture::textureMap;

mage::Texture::Texture():
m_filePath("./assets/textures/default.png"),
m_type("diffuse")
{
	load(m_filePath.c_str());
}

mage::Texture::Texture(const char* filePath) :
		m_filePath(filePath),
		m_type("diffuse")
{
	fprintf(stderr, "Loading default diffuse texture\n");
	load(filePath);
}

mage::Texture::Texture(const char* filePath, const char* type) :
		m_filePath(filePath),
		m_type(type)
{
	if (strcmp(type, TEXTURE_DIFFUSE) != 0 && strcmp(type, TEXTURE_SPECULAR) != 0)
	{
		fprintf(stderr, "Texture type %s is unknown to the Mage3D engine\n", type);
		//throw TexException("Invalid texture type",
		//				   fmt::format("Supplied type: {} - Expected {} or {}", type, TEXTURE_DIFFUSE,
		//							   TEXTURE_SPECULAR).c_str());
	}
	fprintf(stderr, "Loading file %s of type %s\n", filePath, type);
	load(filePath);
}

mage::Texture::~Texture()
{
	if(m_textureData && m_textureData->removeReference())
	{
		if(m_filePath.length() > 0)
			textureMap.erase(m_filePath);
		delete m_textureData;
	}
}

void mage::Texture::enable(uint slot)
{
	assert(slot < 31 && slot >= 0);
	glActiveTexture(GL_TEXTURE0 + slot);
	m_textureData->bind(0);
}

void mage::Texture::disable(uint slot)
{
	assert(slot < 31 && slot >= 0);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void mage::Texture::load(const char* filePath)
{
	std::map<std::string, TextureData*>::const_iterator it = textureMap.find(filePath);
	if(it != textureMap.end())
	{
		m_textureData = it->second;
		m_textureData->addReference();
	}else
	{
		int width, height;
		ubyte* image = SOIL_load_image(filePath, &width, &height, 0, SOIL_LOAD_RGBA);
		if (!image)
		{
			println(console::RED, "Image is null!");
		}
		m_textureData = new TextureData(GL_TEXTURE_2D, width, height, 1, &image, true);
		SOIL_free_image_data(image);
		textureMap.insert(std::pair<std::string, TextureData*>(filePath, m_textureData));
	}
}

std::ostream& mage::operator<<(std::ostream& os, const mage::Texture& texture)
{
	os << "m_filePath: " << texture.m_filePath << " m_type: " << texture.m_type << " m_textureData: "
	   << *(texture.m_textureData);
	return os;
}


mage::TextureData::TextureData(GLenum textureTarget, int width, int height, int numTextures, ubyte** data, bool clamp)
{
	m_id = new uint[numTextures];
	m_numTextures = numTextures;
	m_textureTarget = textureTarget;
	m_width = width;
	m_height = height;

	glGenTextures(m_numTextures, m_id);
	for(int i = 0; i < m_numTextures; i++)
	{
		glBindTexture(m_textureTarget, m_id[i]);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if(clamp)
		{
        	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

        glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);

        glTexImage2D(m_textureTarget, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
	}
}

mage::TextureData::~TextureData()
{
	if(*m_id) glDeleteTextures(m_numTextures, m_id);
	if(m_id) delete[] m_id;
}

#include <iostream>
void mage::TextureData::bind(int textureNum)
{
	glBindTexture(m_textureTarget, m_id[textureNum]);
}

std::ostream& mage::operator<<(std::ostream& os, const mage::TextureData& data)
{
	os << " m_textureTarget: " << data.m_textureTarget
	   << " m_id: " << data.m_id[0] << " m_numTextures: " << data.m_numTextures << " m_width: " << data.m_width
	   << " m_height: " << data.m_height;
	return os;
}
