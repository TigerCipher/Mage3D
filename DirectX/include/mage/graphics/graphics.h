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
 * File Name: renderengine.h
 * Date File Created: 9/16/2020 at 5:17 PM
 * Author: Matt
 */

#ifndef MAGE3DX_GRAPHICS_H
#define MAGE3DX_GRAPHICS_H

#include "mage/debug/graphicsexception.h"
#include <d3d11.h>

namespace mage
{
    class Graphics
    {
    public:
        explicit Graphics(HWND hwnd);
        virtual ~Graphics();

        Graphics(const Graphics& rhs) = delete;
        Graphics& operator=(const Graphics& rhs) = delete;

        void swap();
        void clear(float r, float g, float b) noexcept;
    protected:
    private:
        ID3D11Device* m_device { };
        IDXGISwapChain* m_swap { };
        ID3D11DeviceContext* m_context { };
        ID3D11RenderTargetView* m_target { };
        #ifndef NDEBUG
        DebugInfo m_debugInfo;
        #endif
    };

}


#endif //MAGE3DX_GRAPHICS_H
