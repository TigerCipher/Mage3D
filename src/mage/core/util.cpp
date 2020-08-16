/*
 * Mage3D
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
 * File Name: util.cpp
 * Date File Created: 8/15/2020 at 6:22 PM
 * Author: Matt
 */

#include "mage/core/util.h"

list<std::string> mage::Util::split(const std::string& str, char delim)
{
    list<std::string> elems;
    const char* cstr = str.c_str();
    uint strLen = str.length();
    uint start = 0;
    uint end = 0;
    while(end <= strLen)
    {
        while(end <= strLen)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }

        elems.push_back(str.substr(start, end - start));
        start = end + 1;
        end = start;
    }
    return elems;
}
