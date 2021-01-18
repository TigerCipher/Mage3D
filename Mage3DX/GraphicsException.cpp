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
//#include "pch.h" -intellisense works better with force include being used
#include "GraphicsException.h"
#include "3rdParty/dxerr.h"

GraphicsException::GraphicsException(int line, const char* file, HRESULT hr,
                                           const list<std::string>& msgs) noexcept:
        MageException(line, file),
        mResult(hr)
{
    for (const auto& m : msgs)
    {
        mInfo += m;
        mInfo.push_back('\n');
    }

    if (!mInfo.empty())
        mInfo.pop_back();
}

const char* GraphicsException::what() const noexcept
{
    std::ostringstream oss;
    oss << getType()
        << "\n[Error Code] 0x" << std::hex << std::uppercase << getErrorCode()
        << std::dec << " (" << (unsigned long) getErrorCode() << ")\n"
        << "[Error String] " << getErrorString()
        << "\n[Description] " << getErrorDescription() << "\n";
    if (!mInfo.empty())
        oss << "\n[Error Info]\n" << getErrorInfo() << "\n\n";
    oss << getOrigin();
    mWhat = oss.str();
    return mWhat.c_str();
}

const char* GraphicsException::getType() const noexcept
{
    return "Mage Graphics Exception";
}

HRESULT GraphicsException::getErrorCode() const noexcept
{
    return mResult;
}

std::string GraphicsException::getErrorString() const noexcept
{
    return std::string((char*)DXGetErrorString(mResult));
}

std::string GraphicsException::getErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(mResult, buf, sizeof(buf));
    return buf;
}

std::string GraphicsException::getErrorInfo() const noexcept
{
    return mInfo;
}


const char* GraphicsDeviceRemovedException::getType() const noexcept
{
    return "Mage Graphics Device Removed Exception - DXGI_ERROR_DEVICE_REMOVED";
}
