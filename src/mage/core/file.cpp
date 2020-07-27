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
 * File Name: file.cpp
 * Date File Created: 7/23/2020 at 4:29 PM
 * Author: Matt
 */

#include "mage/core/file.h"

mage::File::File(const char *path):
m_path(path)
{

}

std::string mage::File::read() {
    FILE* file = fopen(m_path, "rt");
    fseek(file, 0, SEEK_END);
    unsigned long len = ftell(file);
    char* data = new char[len + 1];
    memset(data, 0, len + 1);
    fseek(file, 0, SEEK_SET);
    fread(data, 1, len, file);
    std::string ret(data);
    delete[] data;
    return ret;
}
