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
//#include "pch.h" -intellisense works better with force include being used
#include "DisplayException.h"

const char* DisplayException::what() const noexcept
{
	//std::ostringstream oss;
	//oss << getType() << "\n[Error Code] " << getError() << "\n[Description] " << getErrorString() << "\n"
	//    << getOrigin();
	mWhat = fmt::format("{}\n[Error Code] {}\n[Description] {}\n{}", getType(), getError(), getErrorString(),
		getOrigin());
	return mWhat.c_str();
}




std::string DisplayException::getErrorString() const noexcept
{
	return ExceptionHelper::translateError(mResult);
}


std::string ExceptionHelper::translateError(const HRESULT hr) noexcept
{
	char* msgBuffer = nullptr;
	const auto len = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&msgBuffer), 0,
		nullptr);
	if(len == 0) return "Unknown";
	std::string errStr = msgBuffer;
	LocalFree(msgBuffer);
	return errStr;
}
