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
 * File Name: mageexception.cpp
 * Date File Created: 9/12/2020 at 3:01 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "MageException.h"

const char* MageException::what() const noexcept
{
    //std::ostringstream oss;
    //oss << getType() << "\n" << getOrigin();
    //mWhat = oss.str();
    mWhat = fmt::format("{}\n{}", getType(), getOrigin());
    return mWhat.c_str();
}

const char* MageException::getType() const noexcept
{
	mType = fmt::format("Mage3DX#{}", substrFrom(typeid(*this).name(), 6));
    return mType.c_str();
}

std::string MageException::getOrigin() const noexcept
{
    return fmt::format("[File] {}\n[Line] {}", mFile, mLine);
}

std::string MageException::getStacktrace() const noexcept
{
    if (mShowStack)
    {
        //LOG_ERROR("An exception [{}] was caught. Stack trace:\n{}", getType(), asString());
        //std::ostringstream oss;
        //oss << "[File] " << mFile << "\n[Line] " << mLine << "\n[Stack Trace]\n" << asString();
        //return oss.str();
        return fmt::format("[File] {}\n[Line] {}\n[Stack Trace]\n{}", mFile, mLine, asString());
    }

    return getOrigin();
}
std::string ExceptionHelper::translateError(const HRESULT hr) noexcept
{
    char* msgBuffer = nullptr;
    const auto len = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&msgBuffer), 0,
        nullptr);
    if (len == 0) return "Unknown";
    std::string errStr = msgBuffer;
    LocalFree(msgBuffer);
    return errStr;
}
