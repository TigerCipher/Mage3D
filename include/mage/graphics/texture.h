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
#include <GL/glew.h>

namespace mage
{
	class Texture
	{
	public:
		mage3d_EXPORT Texture(const char* filePath);

		mage3d_EXPORT ~Texture();

		mage3d_EXPORT void enable();
		mage3d_EXPORT void disable();

		mage3d_EXPORT GLuint getId() const { return m_id; }

	protected:
	private:
		GLuint m_id;
		const char* m_filePath;
		int m_width;
		int m_height;
	};

}


#endif //MAGE3D_TEXTURE_H
