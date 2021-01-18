/*
 * Mage3DX
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
 * Date File Created: 9/23/2020 at 9:33 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "TextureException.h"

const char* TextureException::what() const noexcept
{
    mWhat = fmt::format("{}\n[Info] {}", MageException::what(), getInfo());
    return mWhat.c_str();
}

const char* TextureException::getType() const noexcept
{
    return "Mage Texture Exception";
}
