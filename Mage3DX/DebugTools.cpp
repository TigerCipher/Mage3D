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
 * File Name: DebugTools.cpp
 * Date File Created: 10/4/2020 at 8:15 PM
 * Author: Matt
 */

#include "pch.h"
#include "DebugTools.h"

#include <iostream>


#include "winwrapper.h"

bool redirectConsole()
{
	auto result = true;
	FILE* fp;

	if(GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
	{
		if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
			result = false;
		else
			setvbuf(stdin, nullptr, _IONBF, 0);
	}

	if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
	{
		if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
			result = false;
		else
			setvbuf(stdout, nullptr, _IONBF, 0);
	}

	if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
	{
		if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
			result = false;
		else
			setvbuf(stderr, nullptr, _IONBF, 0);
	}

	std::ios::sync_with_stdio(true);

	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();

	return result;
}

bool releaseConsole()
{
	auto result = true;
	FILE* fp;
	if (freopen_s(&fp, "NUL:", "r", stdin) != 0)
		result = false;
	else
		setvbuf(stdin, NULL, _IONBF, 0);

	if (freopen_s(&fp, "NUL:", "w", stdout) != 0)
		result = false;
	else
		setvbuf(stdout, NULL, _IONBF, 0);

	if (freopen_s(&fp, "NUL:", "w", stderr) != 0)
		result = false;
	else
		setvbuf(stderr, NULL, _IONBF, 0);

	if (!FreeConsole())
		result = false;
	return result;
}

bool createNewConsole(int16_f minLength)
{
	auto result = false;

	releaseConsole();

	if(AllocConsole())
	{
		adjustConsoleBuffer(minLength);
		result = redirectConsole();
	}

	return result;
}

void adjustConsoleBuffer(int16_f minLength)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	if (csbi.dwSize.Y < minLength)
		csbi.dwSize.Y = minLength;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), csbi.dwSize);
}

bool attachParentConsole(int16_f minLength)
{
	auto result = false;

	releaseConsole();

	if(AttachConsole(ATTACH_PARENT_PROCESS))
	{
		adjustConsoleBuffer(minLength);
		result = redirectConsole();
	}

	return result;
}
