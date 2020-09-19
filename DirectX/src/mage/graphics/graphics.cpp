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
#include "mage/debug/infoexception.h"
#include <d3dcompiler.h>

// probably should do this for my other libs since ultimately this won't be built with Cmake for release
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


#define GFX_THROW_NOINFO(hrcall) {HRESULT hr; if( FAILED( hr = (hrcall) ) ) throw GraphicsException( __LINE__,__FILE__,hr ); }
#define GFX_EXCEPT_NOINFO(hr) GraphicsException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
    #define GFX_EXCEPT(hr) GraphicsException(__LINE__, __FILE__, (hr), m_debugInfo.getMessages())
    #define GFX_THROW_INFO(hrcall) {HRESULT hr; m_debugInfo.set(); if(FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr);}
    #define GFX_DEVICE_REMOVED_EXCEPT(hr) GraphicsDeviceRemovedException( __LINE__,__FILE__,(hr), m_debugInfo.getMessages())
    #define GFX_THROW_INFO_ONLY(func) m_debugInfo.set(); (func); {auto l = m_debugInfo.getMessages(); if(!l.empty()) {throw InfoException(__LINE__, __FILE__, l);}}
#else
    #define GFX_EXCEPT(hr) GraphicsException(__LINE__, __FILE__, (hr))
    #define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
    #define GFX_DEVICE_REMOVED_EXCEPT(hr) GraphicsDeviceRemovedException( __LINE__,__FILE__,(hr) )
    #define GFX_THROW_INFO_ONLY(func) (func)
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

    UINT flg = 0;
    #ifndef NDEBUG
    flg |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                                 flg,
                                                 nullptr, 0,
                                                 D3D11_SDK_VERSION, &swapDesc, &m_swap, &m_device, nullptr,
                                                 &m_context));

    COMptr<ID3D11Resource> backBuffer;
    GFX_THROW_INFO(m_swap->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
    GFX_THROW_INFO(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_target));

    //UINT flgs = D3DCOMPILE_ENABLE_STRICTNESS;
    //#ifndef NDEBUG
    //flgs |= D3DCOMPILE_DEBUG;
    //#endif
    //LPCSTR profile = "vs_5_0";
    //COMptr<ID3DBlob> shaderBlob;
    //COMptr<ID3DBlob> errBlob;
    //GFX_THROW_INFO(
    //        D3DCompileFromFile(L"vertex_shader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
    //                           profile, flgs, 0, &shaderBlob, &errBlob));

}

void mage::Graphics::swap()
{
    HRESULT hr;
    #ifndef NDEBUG
    m_debugInfo.set();
    #endif
    if (FAILED(hr = m_swap->Present(1, 0)))
    {
        LOG_ERROR("Swap chain failed. Reasoning: \n{}", fmt::join(m_debugInfo.getMessages(), "\n"));
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
            throw GFX_DEVICE_REMOVED_EXCEPT(m_device->GetDeviceRemovedReason());
        else throw GFX_EXCEPT(hr);
    }
}

void mage::Graphics::clear(float r, float g, float b) noexcept
{
    const float color[] = { r, g, b, 1.0f };
    m_context->ClearRenderTargetView(m_target.Get(), color);
}

void mage::Graphics::drawTriangle()
{
    struct Vertex
    {
        struct
        {
            float x, y;
        } pos;
        struct
        {
            ubyte r, g, b;
            ubyte a = 255;
        } color;
    };
    Vertex verts[] = {
            { 0,     0.5f,  255, 0,   0 },
            { 0.5f,  -0.5f, 0,   255, 0 },
            { -0.5f, -0.5f, 0,   0,   255 },
            { -0.3f, 0.3f,  0,   255, 0 },
            { 0.3f,  0.3f,  0,   0,   255 },
            { 0,     -0.8f, 255, 0,   0 },

            //{ 0.5f, 1.0f },
            //{ 1, 0.5f },
            //{ 0.5f, 0.5f },
    };
    verts[ 0 ].color.g = 255;
    COMptr<ID3D11Buffer> vertexBuffer;
    D3D11_BUFFER_DESC desc = { };
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = sizeof(Vertex);
    desc.ByteWidth = sizeof(verts);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA data = { };
    data.pSysMem = verts;
    GFX_THROW_INFO(m_device->CreateBuffer(&desc, &data, &vertexBuffer));

    // Bind buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0;
    m_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    const ushort ints[] = {
            0, 1, 2,
            0, 2, 3,
            0, 4, 1,
            2, 1, 5
    };
    COMptr<ID3D11Buffer> iBuf;
    D3D11_BUFFER_DESC iDesc = { };
    iDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    iDesc.Usage = D3D11_USAGE_DEFAULT;
    iDesc.CPUAccessFlags = 0;
    iDesc.MiscFlags = 0;
    iDesc.StructureByteStride = sizeof(ushort);
    iDesc.ByteWidth = sizeof(ints);
    D3D11_SUBRESOURCE_DATA isd = { };
    isd.pSysMem = ints;
    GFX_THROW_INFO(m_device->CreateBuffer(&iDesc, &isd, &iBuf));

    m_context->IASetIndexBuffer(iBuf.Get(), DXGI_FORMAT_R16_UINT, 0);

    COMptr<ID3DBlob> blob;
    // Pixel
    COMptr<ID3D11PixelShader> pixelShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"basicPS.cso", &blob));

    GFX_THROW_INFO(
            m_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr,
                                        &pixelShader));
    m_context->PSSetShader(pixelShader.Get(), nullptr, 0);

    // Vertex
    COMptr<ID3D11VertexShader> vertexShader;

    GFX_THROW_INFO(D3DReadFileToBlob(L"basicVS.cso", &blob));

    GFX_THROW_INFO(
            m_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr,
                                         &vertexShader));
    m_context->VSSetShader(vertexShader.Get(), nullptr, 0);


    COMptr<ID3D11InputLayout> layout;
    const D3D11_INPUT_ELEMENT_DESC eleDesc[] = {
            {
                    "Position", 0, DXGI_FORMAT_R32G32_FLOAT,
                    0, 0,                 D3D11_INPUT_PER_VERTEX_DATA, 0
            },
            {
                    "Color",    0, DXGI_FORMAT_R8G8B8A8_UNORM,
                    0, sizeof(float) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0
            },
    };
    GFX_THROW_INFO(m_device->CreateInputLayout(eleDesc, (UINT) std::size(eleDesc), blob->GetBufferPointer(),
                                               blob->GetBufferSize(), &layout));

    m_context->IASetInputLayout(layout.Get());

    m_context->OMSetRenderTargets(1, m_target.GetAddressOf(), nullptr);

    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    D3D11_VIEWPORT vp;
    vp.Width = 1280;
    vp.Height = 720;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 100;
    vp.TopLeftY = 100;
    m_context->RSSetViewports(1, &vp);

    GFX_THROW_INFO_ONLY(m_context->DrawIndexed((UINT) std::size(ints), 0, 0));
}
