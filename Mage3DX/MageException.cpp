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

#include "MageException.h"

const char* mage::MageException::what() const noexcept
{
    m_what = fmt::format("{}\n{}", getType(), getOrigin());
    return m_what.c_str();
}

const char* mage::MageException::getType() const noexcept
{
    return "Mage Exception";
}

std::string mage::MageException::getOrigin() const noexcept
{
    if(m_showStack)
    {
        LOG_ERROR("An exception [{}] was caught. Stack trace:\n{}", getType(), asString());
        return fmt::format("[File] {}\n[Line] {}\n[Stack Trace]\n{}", m_file, m_line, asString());
    }
    return fmt::format("[File] {}\n[Line] {}", m_file, m_line);
}
