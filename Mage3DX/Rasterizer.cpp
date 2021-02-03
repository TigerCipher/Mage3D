/*
 * Mage3DX
 * Copyright (C) 2021 Blue Moon Development. All rights reserved.
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
 * File Name: Rasterizer.cpp
 * Date File Created: 1/30/2021 at 2:41 PM
 * Author: Matt
 */
#include "Rasterizer.h"

#include "GraphicsException.h"
#include "BindableManger.h"


Rasterizer::Rasterizer(Graphics& gfx, bool twoSided) :
mTwoSided(twoSided)
{
	DEBUG_INFO(gfx);

	D3D11_RASTERIZER_DESC desc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	desc.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

	GFX_THROW_INFO(getDevice(gfx)->CreateRasterizerState(&desc, &mRasterizer));
}

void Rasterizer::bind(Graphics& gfx) noexcept
{
	getContext(gfx)->RSSetState(mRasterizer.Get());
}

SharedPtr<Rasterizer> Rasterizer::resolve(Graphics& gfx, bool twoSided)
{
	return BindableManger::resolve<Rasterizer>(gfx, twoSided);
}

std::string Rasterizer::generateUID(bool twoSided)
{
	using namespace std::string_literals;
	return typeid(Rasterizer).name() + "#"s + (twoSided ? "2s" : "1s");
}

std::string Rasterizer::getUID() const noexcept
{
	return generateUID(mTwoSided);
}
