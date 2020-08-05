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
 * File Name: textureexception.cpp
 * Date File Created: 8/4/2020 at 1:15 AM
 * Author: Matt
 */

#include "mage/errors/textureexception.h"

mage::TextureException::TextureException(const char* msg, const char* file, int line, const char* func,
										 const char* info) :
		m_file(file),
		m_line(line),
		m_func(func),
		m_info(info)
{

}
