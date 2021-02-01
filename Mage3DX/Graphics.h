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
 * File Name: Graphics.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "MathHelper.h"
#include "DebugInfo.h"

#include <d3d11.h>
#include <DirectXTK/SpriteFont.h>


class Graphics
{
friend class Bindable;

public:
	explicit Graphics(HWND hwnd, int width, int height);
	virtual ~Graphics() = default;

	Graphics(const Graphics& rhs) = delete;
	Graphics& operator=(const Graphics& rhs) = delete;

	void swap();
	void clear(float r, float g, float b) const noexcept;

	void drawIndexed(UINT numIndices) noexcept(!MAGE_DEBUG);

	void setProjection(const mat4f proj) noexcept { mProjection = proj; }
	[[nodiscard]] mat4f getProjection() const noexcept { return mProjection; }

	void setCamera(const mat4f cam) noexcept { mCamera = cam; }
	[[nodiscard]] mat4f getCamera() const noexcept { return mCamera; }

	void toggleImgui() noexcept { mImguiEnabled = !mImguiEnabled; }
	void enableImgui() noexcept { mImguiEnabled = true; }
	void disableImgui() noexcept { mImguiEnabled = false; }

	[[nodiscard]] bool isImguiEnabled() const noexcept { return mImguiEnabled; }

	void addFont(const std::string& fontName, const std::string& fontFile);
	void drawText(const std::string& fontName, const std::string& text,
	              float x, float y, dx::XMVECTORF32 color = DirectX::Colors::White, float scale = 1.0f, float rotation = 0);
	void outlineText(const std::string& fontName, const std::string& text,
		float x, float y, dx::XMVECTORF32 outlineColor = dx::Colors::Black, dx::XMVECTORF32 color = DirectX::Colors::White, float scale = 1.0f, float rotation = 0);


protected:
private:
	COMptr<ID3D11Device> mDevice { };
	COMptr<IDXGISwapChain> mSwap { };
	COMptr<ID3D11DeviceContext> mContext { };
	COMptr<ID3D11RenderTargetView> mTarget { };
	D3D11_VIEWPORT mViewport;
	COMptr<ID3D11DepthStencilView> mDepthStencilView { };
	COMptr<ID3D11DepthStencilState> mDepthState;
	UniquePtr<DirectX::SpriteBatch> mSpriteBatch;
	std::unordered_map<std::string, UniquePtr<DirectX::SpriteFont> > mFonts;

	mat4f mProjection{ };
	mat4f mCamera{ };
	bool mImguiEnabled = true;

	uint mVsync;

#if MAGE_DEBUG
	DebugInfo mDebugInfo;
#endif
};



