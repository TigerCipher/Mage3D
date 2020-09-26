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
 * File Name: hresultexception.cpp
 * Date File Created: 9/16/2020 at 10:15 PM
 * Author: Matt
 */

#include "graphics_exception.h"
#include "3rdParty/dxerr.h"

mage::GraphicsException::GraphicsException(int line, const char* file, HRESULT hr,
                                           const list<std::string>& msgs) noexcept:
        MageException(line, file),
        m_result(hr)
{
    for (const auto& m : msgs)
    {
        m_info += m;
        m_info.push_back('\n');
    }

    if (!m_info.empty())
        m_info.pop_back();
}

const char* mage::GraphicsException::what() const noexcept
{
    std::ostringstream oss;
    oss << getType()
        << "\n[Error Code] 0x" << std::hex << std::uppercase << getErrorCode()
        << std::dec << " (" << (unsigned long) getErrorCode() << ")\n"
        << "[Error String] " << getErrorString()
        << "\n[Description] " << getErrorDescription() << "\n";
    if (!m_info.empty())
        oss << "\n[Error Info]\n" << getErrorInfo() << "\n\n";
    oss << getOrigin();
    m_what = oss.str();
    return m_what.c_str();
}

const char* mage::GraphicsException::getType() const noexcept
{
    return "Mage Graphics Exception";
}

HRESULT mage::GraphicsException::getErrorCode() const noexcept
{
    return m_result;
}

std::string mage::GraphicsException::getErrorString() const noexcept
{
    return std::string((char*)DXGetErrorString(m_result));
}

std::string mage::GraphicsException::getErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(m_result, buf, sizeof(buf));
    return buf;
}

std::string mage::GraphicsException::getErrorInfo() const noexcept
{
    return m_info;
}


const char* mage::GraphicsDeviceRemovedException::getType() const noexcept
{
    return "Mage Graphics Device Removed Exception - DXGI_ERROR_DEVICE_REMOVED";
}
