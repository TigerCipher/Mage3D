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
 * File Name: imgui_manager.cpp
 * Date File Created: 9/25/2020 at 3:08 PM
 * Author: Matt
 */

#include "mage/graphics/imgui_manager.h"
#include "3rdParty/imgui/imgui_impl_dx11.h"
#include "3rdParty/imgui/imgui_impl_win32.h"

bool mage::ImguiManager::s_enabled = true;

void mage::ImguiManager::initDx11(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    ImGui_ImplDX11_Init(pDevice, pContext);
}

void mage::ImguiManager::initWin32(HWND hwnd)
{
    ImGui_ImplWin32_Init(hwnd);
}
void mage::ImguiManager::destroyWin32()
{
    ImGui_ImplWin32_Shutdown();
}
void mage::ImguiManager::destroyDx11()
{
    ImGui_ImplDX11_Shutdown();
}

LRESULT mage::ImguiManager::wndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void mage::ImguiManager::renderDx11()
{
    if (s_enabled)
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

void mage::ImguiManager::newFrame()
{
    if (s_enabled)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
}
