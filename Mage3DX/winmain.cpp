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
 * File Name: winmain.cpp
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#include "pch.h"
#include "App.h"
#include "Display.h"
#include "MageException.h"
#include "DebugTools.h"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	try
	{
#if MAGE_DEBUG
		createNewConsole(1024);
#endif
		
		Log::init();
		
		auto ret = 0;
		
		{
			// Inner scope so when in Debug mode the display closes and the console remains open
			// This way system("pause") can be used properly
			App app(1920, 1080, "Mage3DX Game Engine");
			ret = app.run();
		}

#if MAGE_DEBUG
		system("pause");
		releaseConsole();
#endif
		return ret;
	} catch (const MageException& e)
	{
		DestroyWindow(GetActiveWindow());
		MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
	} catch (const std::exception& e)
	{
		DestroyWindow(GetActiveWindow());
		MessageBox(nullptr, e.what(), "STD Exception", MB_OK | MB_ICONEXCLAMATION);
	} catch (...)
	{
		DestroyWindow(GetActiveWindow());
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}