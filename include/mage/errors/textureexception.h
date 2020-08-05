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
 * File Name: textureexception.h
 * Date File Created: 8/4/2020 at 1:15 AM
 * Author: Matt
 */

#ifndef MAGE3D_TEXTUREEXCEPTION_H
#define MAGE3D_TEXTUREEXCEPTION_H


#include "mage3d_exported.h"
#include "mage/common.h"

#include <exception>

#define TexException(msg, info) TextureException(msg, __FILE__, __LINE__, __FUNCTION__, info)

namespace mage
{
	class TextureException : public std::exception
	{
	public:
		mage3d_EXPORT TextureException(const char* msg, const char* file, int line, const char* func,
									   const char* info);

		mage3d_EXPORT const char* getFile() const
		{
			return m_file;
		}

		mage3d_EXPORT const char* getFunc() const
		{
			return m_func;
		}

		mage3d_EXPORT const char* getInfo() const
		{
			return m_info;
		}

		mage3d_EXPORT int getLine() const
		{
			return m_line;
		}

	private:
		const char* m_file;
		const char* m_func;
		const char* m_info;
		int m_line;
	};
}


#endif //MAGE3D_TEXTUREEXCEPTION_H
