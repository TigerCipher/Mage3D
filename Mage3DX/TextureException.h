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
 * File Name: textureexception.h
 * Date File Created: 9/23/2020 at 9:33 PM
 * Author: Matt
 */

#ifndef MAGE3DX_TEXTURE_EXCEPTION_H
#define MAGE3DX_TEXTURE_EXCEPTION_H

#include "MageException.h"

namespace mage
{
    class TextureException : public MageException
    {
    public:
        TextureException(int line, const char* file, std::string info) noexcept :
            MageException(line, file), m_info(std::move(info))
        { }
        const char* what() const noexcept override;
        const char* getType() const noexcept override;
        const std::string& getInfo() const noexcept { return m_info; }

    protected:
    private:
        std::string m_info;
    };

}// namespace mage


#endif//MAGE3DX_TEXTURE_EXCEPTION_H