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
#include <SOIL/SOIL.h>
#include <cassert>
#include <fmt/format.h>

std::map<std::string, mage::TextureData*> mage::Texture::textureMap;

mage::Texture::Texture() :
		m_filePath(TEXTURE_DEFAULT_DIFFUSE),
		m_type(TEXTURE_DIFFUSE)
{
	load(m_filePath.c_str());
}

mage::Texture::Texture(const char* filePath) :
		m_filePath(filePath),
		m_type(TEXTURE_DIFFUSE)
{
	load(filePath);
}

mage::Texture::Texture(const char* filePath, const char* type) :
		m_filePath(filePath),
		m_type(type)
{
	if (strcmp(type, TEXTURE_DIFFUSE) != 0 && strcmp(type, TEXTURE_SPECULAR) != 0 &&
		strcmp(type, TEXTURE_EMISSION) != 0)
	{
		LOG_WARN("Texture type \'{}\' is unknown to the Mage3D engine. Reverting to default type \'diffuse\'", type);
		m_type = TEXTURE_DIFFUSE;
	}
	load(filePath);
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
	if (it != textureMap.end())
	{
		m_textureData = it->second;
		m_textureData->addReference();
		//DBGPRINT("Ref count %i", m_textureData->getResourceCount());
	} else
	{
	    PROFILER_SCOPE(1);
		LOG_TRACE("Loading texture {}", filePath);
		int width, height;
		//TODO Try updating to SOIL2 to see if that better supports DDS files
		ubyte* image = SOIL_load_image(filePath, &width, &height, nullptr, SOIL_LOAD_RGBA);
		if (!image)
		{
			LOG_WARN("Texture ({}) cannot be loaded, reverting to default texture", filePath);
			m_filePath = fmt::format("./assets/textures/defaults/default_{}.png", m_type);
			const char* newPath = m_filePath.c_str();
			image = SOIL_load_image(newPath, &width, &height, nullptr, SOIL_LOAD_RGBA);
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

void mage::Texture::enable(uint slot) const
{
    assert(slot < 31 && slot >= 0);
	glActiveTexture(GL_TEXTURE0 + slot);
	m_textureData->bind(0);
}

void mage::Texture::disable(uint slot) const
{
    assert(slot < 31 && slot >= 0);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void mage::Texture::destroy()
{
	if (m_textureData && m_textureData->removeReference())
	{
		if (m_filePath.length() > 0)
			textureMap.erase(m_filePath);
		LOG_TRACE("Unloading texture {}", m_filePath);
		delete m_textureData;
	}
}


mage::TextureData::TextureData(GLenum textureTarget, int width, int height, int numTextures, ubyte** data,
							   bool clamp)
{
	m_id = new uint[numTextures];
	m_numTextures = numTextures;
	m_textureTarget = textureTarget;
	m_width = width;
	m_height = height;

	glGenTextures(m_numTextures, m_id);
	for (int i = 0; i < m_numTextures; i++)
	{
		glBindTexture(m_textureTarget, m_id[ i ]);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (clamp)
		{
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);

		glTexImage2D(m_textureTarget, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[ i ]);
	}
}

mage::TextureData::~TextureData()
{
	LOG_TRACE("Deleting texture id {}", *m_id);
	if (*m_id) glDeleteTextures(m_numTextures, m_id);
	delete[] m_id;
}

void mage::TextureData::bind(int textureNum)
{
	glBindTexture(m_textureTarget, m_id[ textureNum ]);
}

std::ostream& mage::operator<<(std::ostream& os, const mage::TextureData& data)
{
	os << " m_textureTarget: " << data.m_textureTarget
	   << " m_id: " << data.m_id[ 0 ] << " m_numTextures: " << data.m_numTextures << " m_width: "
	   << data.m_width
	   << " m_height: " << data.m_height;
	return os;
}
