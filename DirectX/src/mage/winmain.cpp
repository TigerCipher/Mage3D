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
 * Date File Created: 9/10/2020 at 3:36 PM
 * Author: Matt
 */

#include <sstream>
#include "mage/debug/mageexception.h"
#include "mage/core/display.h"

using namespace mage;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrefInstance, LPSTR lpCmdLine, int nCmdShow)
{
    try{
        Display display(1920, 1080, "Mage3DX Game Engine");
        MSG msg;
        BOOL res;

        while ((res = GetMessage(&msg, nullptr, 0, 0)) > 0)
        {
            TranslateMessage(&msg); // Gives the WM_CHAR message
            DispatchMessage(&msg);
        }
        if (res == -1) return -1;
        else return msg.wParam;
    }
    catch(const MageException& e)
    {
        MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch(const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "STD Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch(...)
    {
        MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}