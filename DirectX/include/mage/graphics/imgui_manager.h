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
 * File Name: imguimanager.h
 * Date File Created: 9/24/2020 at 2:00 PM
 * Author: Matt
 */

#ifndef MAGE3DX_IMGUI_MANAGER_H
#define MAGE3DX_IMGUI_MANAGER_H

#include "3rdParty/imgui/imgui.h"
#include <d3d11.h>

//#define _GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, N, ...) N
//#define COUNT_VARARGS(...) _GET_NTH_ARG(__VA_ARGS__, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define IMGUI_WRAP(name, ...)       \
if(mage::ImguiManager::isEnabled()) \
{                                   \
    ImGui::Begin(name);             \
    (__VA_ARGS__);                  \
    ImGui::End();                   \
}



namespace mage
{
    class ImguiManager
    {
    public:

        static void start()
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark();
        }

        static void stop()
        {
            ImGui::DestroyContext();
        }

        static void initDx11(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
        static void initWin32(HWND hwnd);
        static void destroyWin32();
        static void destroyDx11();

        static LRESULT  wndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        static void renderDx11();
        static void newFrame();

        static bool isEnabled() noexcept { return s_enabled; }

        static void enable() noexcept { s_enabled = true; }
        static void disable() noexcept { s_enabled = false; }
        static void toggle() noexcept { s_enabled = !s_enabled; }


    private:
        static bool s_enabled;
    };
}

#endif//MAGE3DX_IMGUI_MANAGER_H
