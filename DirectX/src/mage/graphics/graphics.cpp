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

#define MATH_HELPER_IMPL

#include "mage/core/mathhelper.h"

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
    swapDesc.BufferDesc.Width = 1920;
    swapDesc.BufferDesc.Height = 1080;
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

    D3D11_DEPTH_STENCIL_DESC depth = { };
    depth.DepthEnable = TRUE;
    depth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depth.DepthFunc = D3D11_COMPARISON_LESS;

    COMptr<ID3D11DepthStencilState> depthState;
    GFX_THROW_INFO(m_device->CreateDepthStencilState(&depth, &depthState));

    m_context->OMSetDepthStencilState(depthState.Get(), 1);

    COMptr<ID3D11Texture2D> depthStencil;
    D3D11_TEXTURE2D_DESC texDesc = { };
    texDesc.Width = 1920;
    texDesc.Height = 1080;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_D32_FLOAT;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_INFO(m_device->CreateTexture2D(&texDesc, nullptr, &depthStencil));

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    GFX_THROW_INFO(m_device->CreateDepthStencilView(depthStencil.Get(), &dsvDesc, &m_depthStencilView));

    m_context->OMSetRenderTargets(1, m_target.GetAddressOf(), m_depthStencilView.Get());

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
    m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void mage::Graphics::drawTriangle(float angle, float x, float y)
{
    struct Vertex
    {
        struct
        {
            float x, y, z;
        } pos { };
    };

    Vertex verts[] = {
            { -1.0f, -1.0f, -1.0f },
            { 1.0f,  -1.0f, -1.0f },
            { -1.0f, 1.0f,  -1.0f },
            { 1.0f,  1.0f,  -1.0f },
            { -1.0f, -1.0f, 1.0f },
            { 1.0f,  -1.0f, 1.0f },
            { -1.0f, 1.0f,  1.0f },
            { 1.0f,  1.0f,  1.0f },
    };

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
            0, 2, 1, 2, 3, 1,
            1, 3, 5, 3, 7, 5,
            2, 6, 3, 3, 6, 7,
            4, 5, 7, 4, 7, 6,
            0, 4, 2, 2, 4, 6,
            0, 1, 4, 1, 5, 4
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


    // Constant buffer
    struct ConstantBuffer
    {
        mat4f transform;
    };

    const ConstantBuffer cb = {
            {
                    dx::XMMatrixTranspose(
                            dx::XMMatrixRotationZ(angle) *
                            dx::XMMatrixRotationX(angle) *
                            dx::XMMatrixTranslation(x, 0, y + 4) *
                            dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 10.0f)
                                         )
            }
    };

    struct ColorBuffer
    {
        struct
        {
            float r { }, g { }, b { };
            float a = 1.0f;
        } face_colors[6];
    };

    const ColorBuffer colBuf = {
            {
                    { 1.0f, 0.0f, 1.0f },
                    { 1.0f, 0.0f, 0.0f },
                    { 0.0f, 1.0f, 1.0f },
                    { 0.0f, 0.0f, 1.0f },
                    { 1.0f, 1.0f, 0.0f },
                    { 0.0f, 1.0f, 1.0f }
            }
    };


    COMptr<ID3D11Buffer> cBuf;
    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0;
    cbd.StructureByteStride = 0;
    cbd.ByteWidth = sizeof(cb);
    D3D11_SUBRESOURCE_DATA cbsd = { };
    cbsd.pSysMem = &cb;
    GFX_THROW_INFO(m_device->CreateBuffer(&cbd, &cbsd, &cBuf));

    COMptr<ID3D11Buffer> cBuf2;
    D3D11_BUFFER_DESC colbd;
    colbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    colbd.Usage = D3D11_USAGE_DEFAULT;
    colbd.CPUAccessFlags = 0;
    colbd.MiscFlags = 0;
    colbd.StructureByteStride = 0;
    colbd.ByteWidth = sizeof(colBuf);
    D3D11_SUBRESOURCE_DATA colbsd = { };
    colbsd.pSysMem = &colBuf;
    GFX_THROW_INFO(m_device->CreateBuffer(&colbd, &colbsd, &cBuf2));


    // Bind constant buffer
    m_context->VSSetConstantBuffers(0, 1, cBuf.GetAddressOf());
    m_context->PSSetConstantBuffers(0, 1, cBuf2.GetAddressOf());
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
                    "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,
                    0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
            },
    };
    GFX_THROW_INFO(m_device->CreateInputLayout(eleDesc, (UINT) std::size(eleDesc), blob->GetBufferPointer(),
                                               blob->GetBufferSize(), &layout));

    m_context->IASetInputLayout(layout.Get());

    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    D3D11_VIEWPORT vp;
    vp.Width = 1920;
    vp.Height = 1080;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_context->RSSetViewports(1, &vp);

    GFX_THROW_INFO_ONLY(m_context->DrawIndexed((UINT) std::size(ints), 0, 0));
}
