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
#include <SOIL/SOIL.h>
#include <cassert>

mage::Texture::Texture(const char* filePath):
m_filePath(filePath)
{
	ubyte* image = SOIL_load_image(filePath, &m_width, &m_height, 0, SOIL_LOAD_RGB);
	if(!image)
	{
		println(console::RED, "Image is null!");
	}
	glGenTextures(1, &m_id);
	if(!m_id)
	{
		println(console::RED, "Tex id is null!");
	}
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

mage::Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

void mage::Texture::enable()
{
	assert(m_id < 31 && m_id >= 0);
	glActiveTexture(GL_TEXTURE0 + m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void mage::Texture::disable()
{
	assert(m_id < 31 && m_id >= 0);
	glActiveTexture(GL_TEXTURE0 + m_id);
	glBindTexture(GL_TEXTURE_2D, 0);
}
