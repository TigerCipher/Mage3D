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
 * File Name: displayexception.cpp
 * Date File Created: 9/12/2020 at 3:12 PM
 * Author: Matt
 */

#include "mage/debug/displayexception.h"

const char* mage::DisplayException::what() const noexcept
{
    std::ostringstream oss;
    oss << getType() << "\n[Error Code] " << getError() << "\n[Description] " << getErrorString() << "\n"
        << getOrigin();
    m_what = oss.str();
    return m_what.c_str();
}

const char* mage::DisplayException::getType() const noexcept
{
    return "Mage Display Exception";
}

std::string mage::DisplayException::translateError(HRESULT hr) noexcept
{
    char* msgBuffer = nullptr;
    DWORD len = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&msgBuffer), 0,
            nullptr);
    if(len == 0) return "Unknown";
    std::string errStr = msgBuffer;
    LocalFree(msgBuffer);
    return errStr;
}



std::string mage::DisplayException::getErrorString() const noexcept
{
    return translateError(m_result);
}


