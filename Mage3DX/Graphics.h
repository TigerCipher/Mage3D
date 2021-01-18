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


class Graphics
{
friend class Bindable;

public:
	explicit Graphics(HWND hwnd);
	virtual ~Graphics() = default;

	Graphics(const Graphics& rhs) = delete;
	Graphics& operator=(const Graphics& rhs) = delete;

	void swap();
	void clear(float r, float g, float b) noexcept;

	void drawIndexed(UINT numIndices) noexcept(!MAGE_DEBUG);

	void setProjection(mat4f proj) noexcept { mProjection = proj; }
	[[nodiscard]] mat4f getProjection() const noexcept { return mProjection; }

	void setCamera(mat4f cam) noexcept { mCamera = cam; }
	[[nodiscard]] mat4f getCamera() const noexcept { return mCamera; }

	void toggleImgui() noexcept { mImguiEnabled = !mImguiEnabled; }
	void enableImgui() noexcept { mImguiEnabled = true; }
	void disableImgui() noexcept { mImguiEnabled = false; }

	[[nodiscard]] bool isImguiEnabled() const noexcept { return mImguiEnabled; }


protected:
private:
	COMptr<ID3D11Device> mDevice { };
	COMptr<IDXGISwapChain> mSwap { };
	COMptr<ID3D11DeviceContext> mContext { };
	COMptr<ID3D11RenderTargetView> mTarget { };
	COMptr<ID3D11DepthStencilView> mDepthStencilView { };

	mat4f mProjection{ };
	mat4f mCamera{ };
	bool mImguiEnabled = true;

#if MAGE_DEBUG
	DebugInfo mDebugInfo;
#endif
};



