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
 * File Name: infoexception.h
 * Date File Created: 9/18/2020 at 2:55 PM
 * Author: Matt
 */

#ifndef MAGE3DX_INFO_EXCEPTION_H
#define MAGE3DX_INFO_EXCEPTION_H


//#include "pch.h"
#include "MageException.h"

#if MAGE_DEBUG
    #define GFX_THROW_INFO_ONLY(func)                                       \
        m_debugInfo.set();                                                  \
        (func);                                                             \
        {                                                                   \
            auto l = m_debugInfo.getMessages();                             \
            if (!l.empty()) { throw InfoException(__LINE__, __FILE__, l); } \
        }
#else
    #define GFX_THROW_INFO_ONLY(func) (func)
#endif

namespace mage
{
    class InfoException : public MageException
    {
    public:
        InfoException(int line, const char* file, const list<std::string>& msgs) noexcept;
        const char* what() const noexcept override;
        const char* getType() const noexcept override;
        std::string getErrorInfo() const noexcept;

    private:
        std::string m_info;
    };

}// namespace mage


#endif//MAGE3DX_INFO_EXCEPTION_H
