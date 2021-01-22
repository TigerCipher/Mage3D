/*
 * Mage3DX
 * Copyright (C) 2021 Blue Moon Development. All rights reserved.
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
 * File Name: GDIPlusManager.cpp
 * Date File Created: 1/21/2021 at 7:05 PM
 * Author: Matt
 */


#include "GDIPlusManager.h"

#define FULL_WINTARD

#include "winwrapper.h"

namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>

ULONG_PTR GDIPlusManager::sToken = 0;
int GDIPlusManager::sCount = 0;

GDIPlusManager::GDIPlusManager()
{
	if (sCount++ == 0)
		start();
}

GDIPlusManager::~GDIPlusManager()
{
	if (--sCount == 0)
		stop();
}


void GDIPlusManager::start() noexcept
{
	Gdiplus::GdiplusStartupInput input;
	input.GdiplusVersion = 1;
	input.DebugEventCallback = nullptr;
	input.SuppressBackgroundThread = false;
	Gdiplus::GdiplusStartup(&sToken, &input, nullptr);
}

void GDIPlusManager::stop() noexcept
{
	Gdiplus::GdiplusShutdown(sToken);
}
