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
 * File Name: file.h
 * Date File Created: 7/23/2020 at 4:29 PM
 * Author: Matt
 */

#ifndef MAGE3D_FILE_H
#define MAGE3D_FILE_H

#include "mage3d_exported.h"
#include <string>

namespace mage {

    class File {
    public:
        mage3d_EXPORT File(const char* path);

        mage3d_EXPORT ~File() {}

        mage3d_EXPORT std::string read();

        mage3d_EXPORT friend bool operator==(const File &left, const File &right) { return left.m_path == right.m_path; }

        mage3d_EXPORT friend bool operator!=(const File &left, const File &right) { return !(left == right); }

    protected:
    private:
        const char* m_path;
    };
}

#endif //MAGE3D_FILE_H
