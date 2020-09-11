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

#include <Windows.h>
#include <sstream>
#include "mage/common.h"
#include "mage/debug/debugmessagemap.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static mage::DebugMessageMap dmm;
    OutputDebugString(dmm(msg, lParam, wParam).c_str());

    switch (msg)
    {
        case WM_CLOSE:
            PostQuitMessage(69);
            break;
        case WM_KEYDOWN:
            if (wParam == 'K')
            {
                SetWindowText(hWnd, "Key was pressed!");
            }
            break;
        case WM_KEYUP:
            if (wParam == 'K')
            {
                SetWindowText(hWnd, "Key was released!");
            }
            break;
        case WM_CHAR:
        {
            static std::string title;
            title.push_back((char) wParam);
            SetWindowTextA(hWnd, title.c_str());
        }
            break;
        case WM_LBUTTONDOWN:
            POINTS pt = MAKEPOINTS(lParam);
            std::ostringstream oss;
            oss << "(" << pt.x << ", " << pt.y << ")";
            SetWindowText(hWnd, oss.str().c_str());
            break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrefInstance, LPSTR lpCmdLine, int nCmdShow)
{
    PROFILER_START("Window Creation");
    const auto pClassName = "mage3dx";
    // register window class
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = pClassName;
    wc.hIconSm = nullptr;
    RegisterClassEx(&wc);
    // create window instance
    HWND hWnd = CreateWindowEx(
            0, pClassName,
            "Mage3DX Engine",
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
            200, 200, 640, 480,
            nullptr, nullptr, hInstance, nullptr
                              );
    PROFILER_END;
    // show the damn window
    ShowWindow(hWnd, SW_SHOW);


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