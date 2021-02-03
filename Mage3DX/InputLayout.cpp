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
//#include "pch.h" -intellisense works better with force include being used
#include "InputLayout.h"
#include "GraphicsException.h"
#include "BindableManger.h"

InputLayout::InputLayout(Graphics& gfx, vtx::VertexLayout layout,
                         ID3DBlob* vertexBytecode) :
	mVertexLayout(std::move(layout))
{
	DEBUG_INFO(gfx);

	const auto d3dLayout = mVertexLayout.getD3dLayout();
	GFX_THROW_INFO(getDevice(gfx)->CreateInputLayout(d3dLayout.data(), static_cast<UINT>(d3dLayout.size()),
		vertexBytecode->GetBufferPointer(),
		vertexBytecode->GetBufferSize(),
		&mLayout));
}


void InputLayout::bind(Graphics& gfx) noexcept
{
	getContext(gfx)->IASetInputLayout(mLayout.Get());
}

SharedPtr<InputLayout> InputLayout::resolve(Graphics& gfx, const vtx::VertexLayout& layout, ID3DBlob* vsBytecode)
{
	return BindableManger::resolve<InputLayout>(gfx, layout, vsBytecode);
}

std::string InputLayout::generateUID(const vtx::VertexLayout& layout, ID3DBlob* vsBytecode)
{
	using namespace std::string_literals;
	return typeid(InputLayout).name() + "#"s + layout.getCode();
}

std::string InputLayout::getUID() const noexcept
{
	return generateUID(mVertexLayout);
}
