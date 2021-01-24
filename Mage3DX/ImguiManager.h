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
 * File Name: ImguiManager.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "3rdParty/imgui/imgui.h"


#include <d3d11.h>


#ifdef MAGE_IMGUI

#define IMGUI_BEGIN(name, ...)						\
	if(ImguiManager::isEnabled())					\
	{												\
		if(ImGui::Begin(name, __VA_ARGS__)) {

#define IMGUI_END } ImGui::End(); }

#define IMGUI_FUNC(funcName) ImGui::funcName

#define IMGUI_FUNC_COND(funcName) if(ImGui::funcName)

#define IMGUI_FUNC_R(funcName, defaultValue) ImGui::funcName

#else
#define IMGUI_BEGIN(...) if(false) { if(false) {
#define IMGUI_END } }
#define IMGUI_FUNC(...)
#define IMGUI_FUNC_COND(...) if(false)
IMGUI_FUNC_R(funcName, defaultValue) defaultValue

#endif

class ImguiManager
{
public:

	static void start()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		//ImGui::GetIO().MouseDrawCursor = true;
	}

	static void stop()
	{
		destroyDx11();
		destroyWin32();
		ImGui::DestroyContext();
	}

	static void initDx11(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static void initWin32(HWND hwnd);
	static void destroyWin32();
	static void destroyDx11();

	static LRESULT wndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static void renderDx11();
	static void newFrame();

	static bool isEnabled() noexcept { return sEnabled; }

	static void enable() noexcept { sEnabled = true; }
	static void disable() noexcept { sEnabled = false; }
	static void toggle() noexcept { sEnabled = !sEnabled; }

private:
	static bool sEnabled;
};

