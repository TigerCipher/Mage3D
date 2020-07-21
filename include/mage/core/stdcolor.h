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
 * File Name: stdcolor.h
 * Date File Created: 7/20/2020 at 10:40 PM
 * Author: Matt
 */

#ifndef MAGE3D_STDCOLOR_H
#define MAGE3D_STDCOLOR_H

#include "mage3d_exported.h"
#include "mage/core/platform.h"

#ifdef OS_WINDOWS
#include <Windows.h>

#endif

#include <fmt/core.h>
#include <fmt/color.h>


namespace mage {
    namespace std {
        struct Color {
            bool bright;
            uByte fg;
            uByte bg;
        };

        extern mage3d_EXPORT const Color RED;

        template<typename S, typename... Args, typename Char = fmt::char_t<S>>
        void print(Color color, const S &format, Args &&... args) {
#ifdef OS_WINDOWS
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            fmt::print("Current attribs are: {}", csbi.wAttributes);
            uByte fg = color.bright ? color.fg + 8 : color.fg;
            WORD attrib = fg + (color.bg * 16);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attrib);
            fmt::print(format, args...);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
#else
            fmt::print(getStyle(color), format, args...);
#endif

            MAGE3D_NO_EXPORT fmt::text_style getStyle(Color color);
        }
    }
}

#endif //MAGE3D_STDCOLOR_H