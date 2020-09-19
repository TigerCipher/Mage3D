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
 * File Name: hresultexception.h
 * Date File Created: 9/16/2020 at 10:15 PM
 * Author: Matt
 */

#ifndef MAGE3DX_GRAPHICSEXCEPTION_H
#define MAGE3DX_GRAPHICSEXCEPTION_H

#include <utility>

#include "pch.h"
#include "mageexception.h"
#include "mage/debug/debuginfo.h"

namespace mage
{
    class GraphicsException : public MageException
    {
    public:
        GraphicsException(int line, const char* file, HRESULT hr, const list<std::string>& msgs = {}) noexcept;

        const char* what() const noexcept override;
        const char* getType() const noexcept override;
        HRESULT getErrorCode() const noexcept;
        std::string getErrorString() const noexcept;
        std::string getErrorDescription() const noexcept;
        std::string getErrorInfo() const noexcept;
    private:
        HRESULT m_result;
        std::string m_info;
    };

    class GraphicsDeviceRemovedException : public GraphicsException
    {
    public:
        GraphicsDeviceRemovedException(int line, const char* file, HRESULT hr, list<std::string> msgs) noexcept:
                GraphicsException(line, file, hr, std::move(msgs)) { }

        const char* getType() const noexcept override;
    };

}


#endif //MAGE3DX_GRAPHICSEXCEPTION_H
