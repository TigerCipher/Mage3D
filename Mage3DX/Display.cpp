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
 * File Name: Display.cpp
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "Display.h"
#include "resource.h"
#include "ImguiManager.h"


#if MAGE_VERBOSE
	#include "DebugMessageMap.h"
#endif


Display::Window Display::Window::sWinClass;

Display::Window::Window() noexcept:
	m_hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = handleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName();
	wc.hIconSm = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Display::Window::~Window()
{
	UnregisterClass(sWinClassName, getInstance());
}

const char* Display::Window::getName() noexcept
{
	return sWinClassName;
}

HINSTANCE Display::Window::getInstance() noexcept
{
	return sWinClass.m_hInst;
}

Display::Display(int width, int height, const char* title)
{
	ImguiManager::start();
	LOG_INFO("Creating display of size: ({}, {})", width, height);
	RECT region;
	// Center our window to the desktop
	GetClientRect(GetDesktopWindow(), &region);
	region.left = (region.right / 2) - (width / 2);
	region.top = (region.bottom / 2) - (height / 2);
	region.right = width + region.left;
	region.bottom = height + region.top;
	LOG_INFO("Centered window to position ({}, {})", region.left, region.top);
	m_width = width;
	m_height = height;
	m_aspectRatio = (float) m_height / (float) m_width;
	if (!AdjustWindowRect(&region, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU, false))
	{
		throw DISPLAY_LAST_EXCEPTION();
	}
	LOG_INFO("Adjusted window to size ({}, {}) to make the render area be ({}, {})",
		region.right - region.left,
		region.bottom - region.top,
		width, height);
	//RECT screenRegion;
	//GetClientRect(GetDesktopWindow(), &screenRegion);
	m_hwnd = CreateWindow(Window::getName(), title, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
		region.left,
		region.top, region.right - region.left, region.bottom - region.top, nullptr,
		nullptr, Window::getInstance(), this);

	if (!m_hwnd)
	{
		throw DISPLAY_LAST_EXCEPTION();
	}
	ShowWindow(m_hwnd, SW_SHOWDEFAULT);

	ImguiManager::initWin32(m_hwnd);

	m_gfx = createScope<Graphics>(m_hwnd);
}

Display::~Display()
{
	DestroyWindow(m_hwnd);
}

LRESULT CALLBACK Display::handleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
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
CALLBACK Display::handleMessageIntermediate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	auto* const pDisplay = reinterpret_cast<Display*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pDisplay->handleMessage(hWnd, msg, wParam, lParam);
}

LRESULT Display::handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
#if MAGE_VERBOSE
	static DebugMessageMap dmm;
	OutputDebugString(dmm(msg, lParam, wParam).c_str());
#endif

	if (ImguiManager::wndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	const auto& imio = ImGui::GetIO();

	switch (msg)
	{
	case WM_CLOSE:
		LOG_INFO("Quit message received. Closing program");
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (imio.WantCaptureKeyboard) break;
		if (!(lParam & 0x40000000) || keyboard.isAutoRepeat())
		{
			keyboard.onKeyPressed(static_cast<uchar>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (imio.WantCaptureKeyboard) break;
		keyboard.onKeyReleased(static_cast<uchar>(wParam));
		break;
	case WM_CHAR:
		if (imio.WantCaptureMouse) break;
		keyboard.onChar(static_cast<char>(wParam));
		break;
	case WM_MOUSEMOVE:
	{
		if (imio.WantCaptureMouse) break;
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
		{
			mouse.onMouseMove(pt.x, pt.y);
			if (!mouse.isInWindow())
			{
				SetCapture(m_hwnd);
				mouse.onMouseEnter();
			}
		} else
		{
			if (wParam & (MK_RBUTTON | MK_LBUTTON))
			{
				mouse.onMouseMove(pt.x, pt.y);
			} else
			{
				ReleaseCapture();
				mouse.onMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (imio.WantCaptureMouse) break;
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		if (imio.WantCaptureMouse) break;
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onLeftReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			mouse.onMouseLeave();
		}
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (imio.WantCaptureMouse) break;
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		if (imio.WantCaptureMouse) break;
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onRightReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			mouse.onMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		if (imio.WantCaptureMouse) break;
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.onWheelDelta(pt.x, pt.y, delta);
		break;
	}
	case WM_KILLFOCUS:
		keyboard.clearState();
		break;
	default:     // Just so clang-d stops giving me a warning
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Display::setTitle(const std::string& title)
{
	if (!SetWindowText(m_hwnd, title.c_str()))
	{
		throw DISPLAY_LAST_EXCEPTION();
	}
}

std::optional<int> Display::processMessages() noexcept
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

Graphics& Display::getGraphics()
{
	if (!m_gfx) throw DISPLAY_NO_GFX_EXCEPTION();
	return *m_gfx;
}
