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
 * File Name: display.cpp
 * Date File Created: 9/11/2020 at 3:03 PM
 * Author: Matt
 */

#include "mage/core/display.h"
#include "mage/debug/debugmessagemap.h"
#include "mage/resource.h"
#include "../../3rdParty/imgui/imgui_impl_win32.h"


mage::Display::Window mage::Display::Window::s_winClass;

mage::Display::Window::Window() noexcept:
        m_hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = handleMessageSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = getInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = getName();
    wc.hIconSm = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
    RegisterClassEx(&wc);
}

mage::Display::Window::~Window()
{
    UnregisterClass(winClassName, getInstance());
}

const char* mage::Display::Window::getName() noexcept
{
    return winClassName;
}

HINSTANCE mage::Display::Window::getInstance() noexcept
{
    return s_winClass.m_hInst;
}

mage::Display::Display(int width, int height, const char* title)
{
    LOG_INFO("Creating display of size: ({}, {})", width, height);
    //LOG_TRACE("Enabling Process DPI awareness");
    //if(!SetProcessDPIAware())
    //{
    //    throw DISPLAY_LAST_EXCEPTION();
    //}
    LOG_TRACE("Is process dpi aware? {}", IsProcessDPIAware());
    RECT region;
    // Center our window to the desktop
    GetClientRect(GetDesktopWindow(), &region);
    region.left = (region.right / 2) - (width / 2);
    region.top = (region.bottom / 2) - (height / 2);
    region.right = width + region.left;
    region.bottom = height + region.top;
    m_width = width;
    m_height = height;
    m_aspectRatio = (float) m_height / (float) m_width;
    if (!AdjustWindowRect(&region, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false))
    {
        throw DISPLAY_LAST_EXCEPTION();
    }
    //RECT screenRegion;
    //GetClientRect(GetDesktopWindow(), &screenRegion);
    m_hwnd = CreateWindow(Window::getName(), title, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU, region.left,
                          region.top, width, height, nullptr,
                          nullptr, Window::getInstance(), this);

    if (!m_hwnd)
    {
        throw DISPLAY_LAST_EXCEPTION();
    }
    ShowWindow(m_hwnd, SW_SHOWDEFAULT);

    ImGui_ImplWin32_Init(m_hwnd);

    m_gfx = createScope<Graphics>(m_hwnd);
}

mage::Display::~Display()
{
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(m_hwnd);
}

LRESULT CALLBACK mage::Display::handleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        auto* const pDisplay = static_cast<Display*>(cs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pDisplay));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Display::handleMessageIntermediate));
        return pDisplay->handleMessage(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT
CALLBACK mage::Display::handleMessageIntermediate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    auto* const pDisplay = reinterpret_cast<Display*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pDisplay->handleMessage(hWnd, msg, wParam, lParam);
}

LRESULT mage::Display::handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    #if MAGE_VERBOSE
    static mage::DebugMessageMap dmm;
    OutputDebugString(dmm(msg, lParam, wParam).c_str());
    #endif

    if(ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    switch (msg)
    {
        case WM_CLOSE:
            LOG_INFO("Quit message received. Closing program");
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (!(lParam & 0x40000000) || m_keyboard.isAutoRepeat())
            {
                m_keyboard.onKeyPressed(static_cast<uchar>(wParam));
            }
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            m_keyboard.onKeyReleased(static_cast<uchar>(wParam));
            break;
        case WM_CHAR:
            m_keyboard.onChar(static_cast<char>(wParam));
            break;
        case WM_MOUSEMOVE:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
            {
                m_mouse.onMouseMove(pt.x, pt.y);
                if (!m_mouse.isInWindow())
                {
                    SetCapture(m_hwnd);
                    m_mouse.onMouseEnter();
                }
            } else
            {
                if (wParam & (MK_RBUTTON | MK_LBUTTON))
                {
                    m_mouse.onMouseMove(pt.x, pt.y);
                } else
                {
                    ReleaseCapture();
                    m_mouse.onMouseLeave();
                }
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            m_mouse.onLeftPressed(pt.x, pt.y);
            break;
        }
        case WM_LBUTTONUP:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            m_mouse.onLeftReleased(pt.x, pt.y);
            if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
            {
                ReleaseCapture();
                m_mouse.onMouseLeave();
            }
            break;
        }
        case WM_RBUTTONDOWN:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            m_mouse.onRightPressed(pt.x, pt.y);
            break;
        }
        case WM_RBUTTONUP:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            m_mouse.onRightReleased(pt.x, pt.y);
            if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
            {
                ReleaseCapture();
                m_mouse.onMouseLeave();
            }
            break;
        }
        case WM_MOUSEWHEEL:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            m_mouse.onWheelDelta(pt.x, pt.y, delta);
            break;
        }
        case WM_KILLFOCUS:
            m_keyboard.clearState();
            break;
        default: // Just so clang-d stops giving me a warning
            break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void mage::Display::setTitle(const std::string& title)
{
    if (!SetWindowText(m_hwnd, title.c_str()))
    {
        throw DISPLAY_LAST_EXCEPTION();
    }
}

std::optional<int> mage::Display::processMessages() noexcept
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return (int) msg.wParam;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return { };
}

mage::Graphics& mage::Display::getGraphics()
{
    if (!m_gfx) throw DISPLAY_NO_GFX_EXCEPTION();
    return *m_gfx;
}
