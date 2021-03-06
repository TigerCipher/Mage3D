﻿/*
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
 * File Name: Blender.cpp
 * Date File Created: 1/29/2021 at 8:20 PM
 * Author: Matt
 */
#include "Blender.h"

#include "BindableManger.h"
#include "GraphicsException.h"

Blender::Blender(Graphics& gfx, bool blending, std::optional<float> factor) :
	mBlending(blending)
{
	DEBUG_INFO(gfx);

	if(factor)
	{
		mFactors.emplace();
		mFactors->fill(*factor);
	}

	D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{ } };
	auto& brt = blendDesc.RenderTarget[0];

	if(blending)
	{
		brt.BlendEnable = TRUE;

		if(factor)
		{
			brt.SrcBlend = D3D11_BLEND_BLEND_FACTOR;
			brt.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
		}else
		{
			brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		}
		//brt.BlendOp = D3D11_BLEND_OP_ADD;
		//brt.SrcBlendAlpha = D3D11_BLEND_ZERO;
		//brt.DestBlendAlpha = D3D11_BLEND_ZERO;
		//brt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		//brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	GFX_THROW_INFO(getDevice(gfx)->CreateBlendState(&blendDesc, &mBlender));
}

void Blender::bind(Graphics& gfx) noexcept
{
	const float* data = mFactors ? mFactors->data() : nullptr;
	getContext(gfx)->OMSetBlendState(mBlender.Get(), data, 0xFFFFFFFFu);
}

SharedPtr<Blender> Blender::resolve(Graphics& gfx, bool blending, std::optional<float> factor)
{
	return BindableManger::resolve < Blender >(gfx, blending, factor);
}

std::string Blender::generateUID(bool blending, std::optional<float> factor)
{
	using namespace std::string_literals;
	return typeid(Blender).name() + "#"s + (blending ? "b"s : "nb"s) + (factor ? "#f"s + std::to_string(*factor) : "");
}

std::string Blender::getUID() const noexcept
{
	return generateUID(mBlending, mFactors ? mFactors->front() : std::optional<float>{});
}


void Blender::setFactor(float factor) NOX
{
	assert(mFactors);
	mFactors->fill(factor);
}

float Blender::getFactor() const NOX
{
	assert(mFactors);
	return mFactors->front();
}
