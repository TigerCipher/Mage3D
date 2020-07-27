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
 * File Name: stdcolor.cpp
 * Date File Created: 7/20/2020 at 10:44 PM
 * Author: Matt
 */


#include "mage/core/stdcolor.h"


namespace mage
{


	const Color WHITE = { false, 7, 0, true };
	const Color RED = { false, 4, 0, true };
	const Color RED_ON_GREEN = { false, 4, 2 };

	uByte background = 0;

	fmt::text_style getStyle(Color color)
	{
		uByte actualBg = color.bright ? color.bg + 8 : color.bg;
		fmt::terminal_color bg;
		if (actualBg == 0) bg = fmt::terminal_color::black;
		else if (actualBg == 1) bg = fmt::terminal_color::blue;
		else if (actualBg == 2) bg = fmt::terminal_color::green;
		else if (actualBg == 3) bg = fmt::terminal_color::cyan;
		else if (actualBg == 4) bg = fmt::terminal_color::red;
		else if (actualBg == 5) bg = fmt::terminal_color::magenta;
		else if (actualBg == 6) bg = fmt::terminal_color::yellow;
		else if (actualBg == 7) bg = fmt::terminal_color::white;
		else if (actualBg == 8) bg = fmt::terminal_color::bright_black;
		else if (actualBg == 9) bg = fmt::terminal_color::bright_blue;
		else if (actualBg == 10) bg = fmt::terminal_color::bright_green;
		else if (actualBg == 11) bg = fmt::terminal_color::bright_cyan;
		else if (actualBg == 12) bg = fmt::terminal_color::bright_red;
		else if (actualBg == 13) bg = fmt::terminal_color::bright_magenta;
		else if (actualBg == 14) bg = fmt::terminal_color::bright_yellow;
		else if (actualBg == 15) bg = fmt::terminal_color::bright_white;

		uByte actualFg = color.bright ? color.fg + 8 : color.fg;
		fmt::terminal_color fg;
		if (actualFg == 0) fg = fmt::terminal_color::black;
		else if (actualFg == 1) fg = fmt::terminal_color::blue;
		else if (actualFg == 2) fg = fmt::terminal_color::green;
		else if (actualFg == 3) fg = fmt::terminal_color::cyan;
		else if (actualFg == 4) fg = fmt::terminal_color::red;
		else if (actualFg == 5) fg = fmt::terminal_color::magenta;
		else if (actualFg == 6) fg = fmt::terminal_color::yellow;
		else if (actualFg == 7) fg = fmt::terminal_color::white;
		else if (actualFg == 8) fg = fmt::terminal_color::bright_black;
		else if (actualFg == 9) fg = fmt::terminal_color::bright_blue;
		else if (actualFg == 10) fg = fmt::terminal_color::bright_green;
		else if (actualFg == 11) fg = fmt::terminal_color::bright_cyan;
		else if (actualFg == 12) fg = fmt::terminal_color::bright_red;
		else if (actualFg == 13) fg = fmt::terminal_color::bright_magenta;
		else if (actualFg == 14) fg = fmt::terminal_color::bright_yellow;
		else if (actualFg == 15) fg = fmt::terminal_color::bright_white;

		return fmt::fg(fg) | fmt::bg(bg);
	}

	void clearConsole(Color color)
	{
#ifdef OS_WINDOWS
		background = color.bg;
		WORD attrib = (color.bg << 4) + color.fg;
		HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord = { 0, 0 };
		DWORD count;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		SetConsoleTextAttribute(stdOut, attrib);
		if (GetConsoleScreenBufferInfo(stdOut, &csbi))
		{
			FillConsoleOutputCharacter(stdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
			FillConsoleOutputAttribute(stdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord,
									   &count);
			SetConsoleCursorPosition(stdOut, coord);
		}
#endif
	}

}
