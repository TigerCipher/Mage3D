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

	void setProjection(mat4f proj) noexcept { m_projection = proj; }
	[[nodiscard]] mat4f getProjection() const noexcept { return m_projection; }

	void setCamera(mat4f cam) noexcept { m_camera = cam; }
	[[nodiscard]] mat4f getCamera() const noexcept { return m_camera; }

	void toggleImgui() noexcept { m_imguiEnabled = !m_imguiEnabled; }
	void enableImgui() noexcept { m_imguiEnabled = true; }
	void disableImgui() noexcept { m_imguiEnabled = false; }

	[[nodiscard]] bool isImguiEnabled() const noexcept { return m_imguiEnabled; }


protected:
private:
	COMptr<ID3D11Device> m_device { };
	COMptr<IDXGISwapChain> m_swap { };
	COMptr<ID3D11DeviceContext> m_context { };
	COMptr<ID3D11RenderTargetView> m_target { };
	COMptr<ID3D11DepthStencilView> m_depthStencilView { };

	mat4f m_projection{ };
	mat4f m_camera{ };
	bool m_imguiEnabled = true;

#if MAGE_DEBUG
	DebugInfo m_debugInfo;
#endif
};



