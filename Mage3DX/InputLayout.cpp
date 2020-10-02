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
 * File Name: inputlayout.cpp
 * Date File Created: 9/20/2020 at 9:50 PM
 * Author: Matt
 */
#include "pch.h"
#include "InputLayout.h"
#include "GraphicsException.h"

mage::InputLayout::InputLayout(mage::Graphics& gfx, const list<D3D11_INPUT_ELEMENT_DESC> layout,
                               ID3DBlob* vertexBytecode)
{
    DEBUG_INFO(gfx);

    GFX_THROW_INFO(getDevice(gfx)->CreateInputLayout(layout.data(), (UINT) layout.size(),
                                                     vertexBytecode->GetBufferPointer(),
                                                     vertexBytecode->GetBufferSize(), &m_layout));
}


void mage::InputLayout::bind(mage::Graphics& gfx) noexcept
{
    getContext(gfx)->IASetInputLayout(m_layout.Get());
}
