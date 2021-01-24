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
 * File Name: InfoException.cpp
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "InfoException.h"
#include "Util.h"

InfoException::InfoException(const int line, const char* file, const list<std::string>& msgs) noexcept:
	MageException(line, file)
{
	from_list(mInfo, msgs);
}

const char* InfoException::what() const noexcept
{
	/*std::ostringstream oss;
	oss << getType() << "\n[Error Info]\n" << getErrorInfo() << "\n\n" << getOrigin();
	mWhat = oss.str();*/
	mWhat = fmt::format("{}\n[Error Info]\n{}\n\n{}", getType(), getErrorInfo(), getOrigin());
	return mWhat.c_str();
}

std::string InfoException::getErrorInfo() const noexcept
{
	return mInfo;
}
