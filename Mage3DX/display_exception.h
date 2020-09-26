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
 * File Name: displayexception.h
 * Date File Created: 9/12/2020 at 3:12 PM
 * Author: Matt
 */

#ifndef MAGE3DX_DISPLAY_EXCEPTION_H
#define MAGE3DX_DISPLAY_EXCEPTION_H


#include "mage_exception.h"

#define DISPLAY_EXCEPTION(hr) DisplayException(__LINE__, __FILE__, hr)
#define DISPLAY_LAST_EXCEPTION() DisplayException(__LINE__, __FILE__, GetLastError())
#define DISPLAY_NO_GFX_EXCEPTION() NoGraphicsException(__LINE__, __FILE__)

namespace mage
{
    class ExceptionHelper
    {
    public:
        static std::string translateError(HRESULT hr) noexcept;
    };
    class DisplayException : public MageException
    {
    public:
        DisplayException(int line, const char* file, HRESULT hr) : MageException(line, file), m_result(hr) {}
        const char* what() const noexcept override;
        const char* getType() const noexcept override;
        inline HRESULT getError() const noexcept { return m_result; }
        std::string getErrorString() const noexcept;
    private:
        HRESULT m_result;
    };

    class NoGraphicsException : public MageException
    {
    public:
        NoGraphicsException(int line, const char* file) : MageException(line, file) {}
        const char * getType() const noexcept override;
    };

}


#endif//MAGE3DX_DISPLAY_EXCEPTION_H