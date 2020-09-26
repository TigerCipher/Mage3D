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
 * File Name: infoexception.cpp
 * Date File Created: 9/18/2020 at 2:55 PM
 * Author: Matt
 */

#include "info_exception.h"
#include "util.h"

mage::InfoException::InfoException(int line, const char* file, const list<std::string>& msgs) noexcept:
MageException(line, file)
{
    fromList(m_info, msgs);
}

const char* mage::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << getType() << "\n[Error Info]\n" << getErrorInfo() << "\n\n" << getOrigin();
    m_what = oss.str();
    return m_what.c_str();
}

const char* mage::InfoException::getType() const noexcept
{
    return "Mage Info Exception";
}

std::string mage::InfoException::getErrorInfo() const noexcept
{
    return m_info;
}
