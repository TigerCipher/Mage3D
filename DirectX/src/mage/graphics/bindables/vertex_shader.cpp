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
 * File Name: vertexshader.cpp
 * Date File Created: 9/20/2020 at 10:48 PM
 * Author: Matt
 */

#include "mage/graphics/bindables/vertex_shader.h"
#include "mage/debug/graphics_exception.h"
#include <d3dcompiler.h>

mage::VertexShader::VertexShader(mage::Graphics& gfx, const std::wstring& path)
{
    DEBUG_INFO(gfx);

    GFX_THROW_INFO( D3DReadFileToBlob( path.c_str(),&m_bytecode ) );
	GFX_THROW_INFO( getDevice( gfx )->CreateVertexShader(
		m_bytecode->GetBufferPointer(),
		m_bytecode->GetBufferSize(),
		nullptr,
		&m_shader
	) );
}

void mage::VertexShader::bind(mage::Graphics& gfx) noexcept
{
    getContext(gfx)->VSSetShader(m_shader.Get(), nullptr, 0);
}

ID3DBlob* mage::VertexShader::getBytecode() const noexcept
{
    return m_bytecode.Get();
}
