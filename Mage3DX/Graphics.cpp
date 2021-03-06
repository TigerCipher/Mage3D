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
#include "Graphics.h"
#include "GraphicsException.h"
#include "InfoException.h"


#include "MathHelper.h"

#include "ImguiManager.h"
#include "Util.h"
#include "Settings.h"

// might do this since for release id be using visual studio to build, likely with different
//compiler flags than set up in cmake currently
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "D3DCompiler.lib")



Graphics::Graphics(HWND hwnd, int width, int height)
{
	mVsync = Settings::getBool("Display", "VSync");
	//RECT r;
	//GetClientRect(hwnd, &r);
	//UINT w = r.right - r.left;
	//UINT h = r.bottom - r.top;
	LOG_INFO("Setting swap chain buffer dimensions ({}, {})", width, height);
	DXGI_SWAP_CHAIN_DESC swapDesc = { };
	swapDesc.BufferDesc.Width = 0;
	swapDesc.BufferDesc.Height = 0;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.BufferCount = 1;
	swapDesc.OutputWindow = hwnd;
	swapDesc.Windowed = TRUE;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = 0;

	UINT flg = 0;
#if MAGE_DEBUG
	flg |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		flg,
		nullptr, 0,
		D3D11_SDK_VERSION, &swapDesc, &mSwap, &mDevice, nullptr,
		&mContext));

	COMptr<ID3D11Resource> backBuffer;
	GFX_THROW_INFO(mSwap->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	GFX_THROW_INFO(mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &mTarget));


	LOG_INFO("Setting depth stencil dimensions ({}, {})", width, height);
	COMptr<ID3D11Texture2D> depthStencil;
	D3D11_TEXTURE2D_DESC texDesc = { };
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(mDevice->CreateTexture2D(&texDesc, nullptr, &depthStencil));

	D3D11_DEPTH_STENCIL_DESC depth = { };
	depth.DepthEnable = TRUE;
	depth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth.DepthFunc = D3D11_COMPARISON_LESS;


	/*COMptr<ID3D11DepthStencilState> mDepthState;*/
	GFX_THROW_INFO(mDevice->CreateDepthStencilState(&depth, &mDepthState));


	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	GFX_THROW_INFO(mDevice->CreateDepthStencilView(depthStencil.Get(), &dsvDesc, &mDepthStencilView));

	mContext->OMSetDepthStencilState(mDepthState.Get(), 1);
	mContext->OMSetRenderTargets(1, mTarget.GetAddressOf(), mDepthStencilView.Get());

	LOG_INFO("Setting viewport dimensions ({}, {})", width, height);
	
	mViewport.Width = static_cast<float>(width);
	mViewport.Height = static_cast<float>(height);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mContext->RSSetViewports(1, &mViewport);

	ImguiManager::initDx11(mDevice.Get(), mContext.Get());
	constexpr float uiScale = 1.5f;
	ImGui::GetIO().FontGlobalScale = uiScale;
	ImGui::GetIO().FontAllowUserScaling = true;
	ImGui::GetStyle().ScaleAllSizes(uiScale);

	mSpriteBatch = createScope<DirectX::SpriteBatch>(mContext.Get());
	
}

void Graphics::swap()
{
	ImguiManager::renderDx11();

	HRESULT hr;
#if MAGE_DEBUG
	mDebugInfo.set();
#endif
	if (FAILED(hr = mSwap->Present(mVsync, 0)))
	{
#if MAGE_DEBUG
		const auto debugMsgs = from_list(mDebugInfo.getMessages());
		//LOG_ERROR("Swap chain failed. Reasoning: \n{}", fmt::join(mDebugInfo.getMessages(), "\n"));
		LOG_ERROR("Swap chain failed. Reasoning: \n{}", debugMsgs);
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
			throw GFX_DEVICE_REMOVED_EXCEPT(mDevice->GetDeviceRemovedReason());
		else throw GFX_EXCEPT(hr);
#endif
	}
}

void Graphics::clear(const float r, const float g, const float b) const noexcept
{
	ImguiManager::newFrame();
	const float color[] = { r, g, b, 1.0f };
	
	// Seems like with DirectXTK states must be set each frame to be able to use SpriteBatch for font rendering
	mContext->RSSetViewports(1, &mViewport);
	mContext->OMSetDepthStencilState(mDepthState.Get(), 1);
	mContext->OMSetRenderTargets(1, mTarget.GetAddressOf(), mDepthStencilView.Get());
	
	mContext->ClearRenderTargetView(mTarget.Get(), color);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::drawIndexed(UINT numIndices) noexcept(!MAGE_DEBUG)
{
	GFX_THROW_INFO_ONLY(mContext->DrawIndexed(numIndices, 0, 0));
}


void Graphics::addFont(const std::string& fontName, const std::string& fontFile)
{
	mFonts[fontName] = createScope<DirectX::SpriteFont>(mDevice.Get(),
		std::wstring{ fontFile.begin(), fontFile.end() }.c_str());
	mFonts[fontName]->SetDefaultCharacter(L'?');
	LOG_INFO("Loaded font {} from file [{}]", fontName, fontFile);
}

void Graphics::drawText(const std::string& fontName, const std::string& text,
	const float x, const float y, dx::XMVECTORF32 color, const float scale, const float rotation)
{
	mSpriteBatch->Begin();

	mFonts[fontName]->DrawString(mSpriteBatch.get(), std::wstring{ text.begin(), text.end() }.c_str(),
		{ x, y },
		color, toRadians(rotation),
		{ 0, 0, 0 }, scale);


	mSpriteBatch->End();
}

void Graphics::outlineText(const std::string& fontName, const std::string& text, float x, float y,
	dx::XMVECTORF32 outlineColor, dx::XMVECTORF32 color, float scale, float rotation)
{
	drawText(fontName, text, x + 1, y + 1, outlineColor, scale, rotation);
	drawText(fontName, text, x - 1, y + 1, outlineColor, scale, rotation);
	drawText(fontName, text, x - 1, y - 1, outlineColor, scale, rotation);
	drawText(fontName, text, x + 1, y - 1, outlineColor, scale, rotation);
	
	drawText(fontName, text, x, y, color, scale, rotation);
}
