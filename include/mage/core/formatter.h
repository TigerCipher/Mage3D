// Mage3D
// Copyright (C) 2020 Blue Moon Development
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// 
// Contact: team@bluemoondev.org
// 
// File Name: formatter.h
// Date File Created: 07/17/2020 at 12:57 AM
// Author: Matt / TigerCipher

#pragma once

#include <fmt/color.h>
#include <fmt/core.h>
#include "mage3d_exported.h"

#ifdef WIN32 //|| _WIN32 || _WIN64 || _WINDOWS
#include <Windows.h>
HANDLE g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif // WIN32


// color your text in Windows console mode
// colors are 0=black 1=blue 2=green and so on to 15=bright white  
// colorattribute = foreground + background * 16
// to get red text on yellow use 4 + 14*16 = 228
// light red on yellow would be 12 + 14*16 = 236
//

mage3d_EXPORT struct Color
{
#ifdef WIN32
	WORD attributes;
#else
	fmt::color color;
#endif
};


#ifdef WIN32
const Color RED = { FOREGROUND_RED };
const Color GREEN = { FOREGROUND_GREEN };
const Color BLUE = { FOREGROUND_BLUE };
const Color YELLOW = { FOREGROUND_RED | FOREGROUND_GREEN};
const Color CYAN = { FOREGROUND_BLUE  | FOREGROUND_GREEN};
const Color MAGENTA = { FOREGROUND_BLUE | FOREGROUND_RED };
const Color WHITE = { FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED };

const Color BRIGHT_RED = {FOREGROUND_INTENSITY | RED.attributes};
const Color BRIGHT_GREEN = {FOREGROUND_INTENSITY | GREEN.attributes};
const Color BRIGHT_BLUE = {FOREGROUND_INTENSITY | BLUE.attributes};
const Color BRIGHT_YELLOW = {FOREGROUND_INTENSITY | YELLOW.attributes};
const Color BRIGHT_CYAN = {FOREGROUND_INTENSITY | CYAN.attributes};
const Color BRIGHT_MAGENTA = {FOREGROUND_INTENSITY | MAGENTA.attributes};
const Color BRIGHT_WHITE = {FOREGROUND_INTENSITY | WHITE.attributes};

#else
const Color RED     = {fmt::color::dark_red};
const Color GREEN   = {fmt::color::dark_green};
const Color BLUE    = {fmt::color::dark_blue};
const Color YELLOW  = {fmt::color::yellow};
const Color CYAN    = {fmt::color::dark_cyan};
const Color MAGENTA = {fmt::color::dark_magenta};
const Color WHITE   = {fmt::color::white_smoke};

const Color BRIGHT_RED     = {fmt::color::red};
const Color BRIGHT_GREEN   = {fmt::color::green};
const Color BRIGHT_BLUE    = {fmt::color::blue};
const Color BRIGHT_YELLOW  = {fmt::color::light_golden_rod_yellow};
const Color BRIGHT_CYAN    = {fmt::color::cyan};
const Color BRIGHT_MAGENTA = {fmt::color::magenta};
const Color BRIGHT_WHITE   = {fmt::color::white};
#endif


template <typename S, typename... Args, typename Char = fmt::char_t<S>>
void print(Color color, const S& text, Args&&...args)
{
#ifdef WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(g_hConsole, &csbi);
	FlushConsoleInputBuffer(g_hConsole);
	SetConsoleTextAttribute(g_hConsole, color.attributes);
	fmt::print(text, args...);
	SetConsoleTextAttribute(g_hConsole, csbi.wAttributes); // Reset the console buffer
#else
		fmt::print(fg(color.color), text, args...);
#endif

}
