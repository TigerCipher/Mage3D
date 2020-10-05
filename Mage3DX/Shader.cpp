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
 * File Name: Shader.cpp
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#include "pch.h"
#include "Shader.h"
#include "GraphicsException.h"
#include <d3dcompiler.h>


mage::VertexShader::VertexShader(mage::Graphics& gfx, const std::wstring& path)
{
	DEBUG_INFO(gfx);

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &m_bytecode));
	GFX_THROW_INFO(getDevice(gfx)->CreateVertexShader(
		m_bytecode->GetBufferPointer(),
		m_bytecode->GetBufferSize(),
		nullptr,
		&m_shader
		));
}

void mage::VertexShader::bind(mage::Graphics& gfx) noexcept
{
	getContext(gfx)->VSSetShader(m_shader.Get(), nullptr, 0);
}


mage::PixelShader::PixelShader(mage::Graphics& gfx, const std::wstring& path)
{
	DEBUG_INFO(gfx);

	COMptr<ID3DBlob> blob;
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &blob));
	GFX_THROW_INFO(getDevice(gfx)->CreatePixelShader(blob->GetBufferPointer(),
		blob->GetBufferSize(), nullptr, &m_shader));
}


void mage::PixelShader::bind(mage::Graphics& gfx) noexcept
{
	getContext(gfx)->PSSetShader(m_shader.Get(), nullptr, 0);
}
