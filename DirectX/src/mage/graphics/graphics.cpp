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
 * File Name: renderengine.cpp
 * Date File Created: 9/16/2020 at 5:17 PM
 * Author: Matt
 */

#include "mage/graphics/graphics.h"

// probably should do this for my other libs since ultimately this won't be built with Cmake for release
#pragma comment(lib, "d3d11.lib")


#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw GraphicsException( __LINE__,__FILE__,hr )
#define GFX_EXCEPT_NOINFO(hr) GraphicsException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
    #define GFX_EXCEPT(hr) GraphicsException(__LINE__, __FILE__, (hr), m_debugInfo.getMessages())
    #define GFX_THROW_INFO(hrcall) m_debugInfo.set(); if(FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
    #define GFX_DEVICE_REMOVED_EXCEPT(hr) GraphicsDeviceRemovedException( __LINE__,__FILE__,(hr), m_debugInfo.getMessages() )
#else
    #define GFX_EXCEPT(hr) GraphicsException(__LINE__, __FILE__, (hr))
    #define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
    #define GFX_DEVICE_REMOVED_EXCEPT(hr) GraphicsDeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif


mage::Graphics::Graphics(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC swapDesc = { };
    swapDesc.BufferDesc.Width = 0;
    swapDesc.BufferDesc.Height = 0;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    // No AA for now
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;

    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.BufferCount = 1;
    swapDesc.OutputWindow = hwnd;
    swapDesc.Windowed = TRUE;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    //swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD ;
    swapDesc.Flags = 0;

    HRESULT hr;
    UINT flg = 0;
    #ifndef NDEBUG
    flg |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                                 flg,
                                                 nullptr, 0,
                                                 D3D11_SDK_VERSION, &swapDesc, &m_swap, &m_device, nullptr,
                                                 &m_context));

    ID3D11Resource* backBuffer;
    GFX_THROW_INFO(m_swap->GetBuffer(0, __uuidof(ID3D11Resource), VOIDPP(&backBuffer)));
    GFX_THROW_INFO(m_device->CreateRenderTargetView(backBuffer, nullptr, &m_target));
    backBuffer->Release();
}

mage::Graphics::~Graphics()
{
    if (m_target)
        m_target->Release();
    if (m_context)
        m_context->Release();
    if (m_swap)
        m_swap->Release();
    if (m_device)
        m_device->Release();
}

void mage::Graphics::swap()
{
    HRESULT hr;
    #ifndef NDEBUG
    m_debugInfo.set();
    #endif
    if (FAILED(hr = m_swap->Present(1, 0)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
            throw GFX_DEVICE_REMOVED_EXCEPT(m_device->GetDeviceRemovedReason());
        else throw GFX_EXCEPT(hr);
    }
}

void mage::Graphics::clear(float r, float g, float b) noexcept
{
    const float color[] = { r, g, b, 1.0f };
    m_context->ClearRenderTargetView(m_target, color);
}
