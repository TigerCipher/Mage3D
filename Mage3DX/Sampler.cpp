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
 * File Name: sampler.cpp
 * Date File Created: 9/23/2020 at 10:37 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "Sampler.h"

#include "BindableManger.h"
#include "GraphicsException.h"
#include "Settings.h"


Sampler::Sampler(Graphics& gfx)
{
    DEBUG_INFO(gfx);
    D3D11_SAMPLER_DESC sd = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
    sd.Filter = D3D11_FILTER_ANISOTROPIC;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    //sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	// AF x4 is default
    const UINT anistropy = Settings::getInt("Graphics", "Anistropy", { 4, 8, 16 });
    sd.MaxAnisotropy = anistropy;
	
    //sd.MipLODBias = 0.0f;
    //sd.MinLOD = 0.0f;
    //sd.MaxLOD = D3D11_FLOAT32_MAX;

    GFX_THROW_INFO(getDevice(gfx)->CreateSamplerState(&sd, &mSampler));
}


void Sampler::bind(Graphics& gfx) noexcept
{
    getContext(gfx)->PSSetSamplers(0, 1, mSampler.GetAddressOf());
}

SharedPtr<Sampler> Sampler::resolve(Graphics& gfx)
{
    return BindableManger::resolve<Sampler>(gfx);
}

std::string Sampler::generateUID()
{
    return typeid(Sampler).name();
}

