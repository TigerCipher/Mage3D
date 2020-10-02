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
 * File Name: graphics.cpp
 * Date File Created: 9/27/2020 at 3:28 PM
 * Author: Matt
 */
#include "pch.h"
#include "Graphics.h"
#include "GraphicsException.h"
#include "InfoException.h"


#include "MathHelper.h"

#include "ImguiManager.h"

// might do this since for release id be using visual studio to build, likely with different
//compiler flags than set up in cmake currently
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "D3DCompiler.lib")


constexpr int vsync_flag = 0;

mage::Graphics::Graphics(HWND hwnd)
{
	RECT r;
	GetClientRect(hwnd, &r);
	UINT w = r.right - r.left;
	UINT h = r.bottom - r.top;
	LOG_INFO("Setting swap chain buffer dimensions ({}, {})", w, h);
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
	swapDesc.Flags = 0;

	UINT flg = 0;
#if MAGE_DEBUG
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

	LOG_INFO("Setting depth stencil dimensions ({}, {})", w, h);
	COMptr<ID3D11Texture2D> depthStencil;
	D3D11_TEXTURE2D_DESC texDesc = { };
	texDesc.Width = w;
	texDesc.Height = h;
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

	LOG_INFO("Setting viewport dimensions ({}, {})", w, h);
	D3D11_VIEWPORT vp;
	vp.Width = (float)w;
	vp.Height = (float)h;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_context->RSSetViewports(1, &vp);

	ImguiManager::initDx11(m_device.Get(), m_context.Get());
	constexpr float uiScale = 1.5f;
	ImGui::GetIO().FontGlobalScale = uiScale;
	ImGui::GetIO().FontAllowUserScaling = true;
	ImGui::GetStyle().ScaleAllSizes(uiScale);
}

void mage::Graphics::swap()
{
	ImguiManager::renderDx11();

	HRESULT hr;
#if MAGE_DEBUG
	m_debugInfo.set();
#endif
	if (FAILED(hr = m_swap->Present(vsync_flag, 0)))
	{
#if MAGE_DEBUG
		LOG_ERROR("Swap chain failed. Reasoning: \n{}", fmt::join(m_debugInfo.getMessages(), "\n"));
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
			throw GFX_DEVICE_REMOVED_EXCEPT(m_device->GetDeviceRemovedReason());
		else throw GFX_EXCEPT(hr);
#endif
	}
}

void mage::Graphics::clear(float r, float g, float b) noexcept
{
	ImguiManager::newFrame();
	const float color[] = { r, g, b, 1.0f };
	m_context->ClearRenderTargetView(m_target.Get(), color);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void mage::Graphics::drawIndexed(UINT numIndices) noexcept(!MAGE_DEBUG)
{
	GFX_THROW_INFO_ONLY(m_context->DrawIndexed(numIndices, 0, 0));
}
