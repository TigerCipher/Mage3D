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

#include "Sampler.h"
#include "GraphicsException.h"


mage::Sampler::Sampler(mage::Graphics& gfx)
{
    DEBUG_INFO(gfx);
    D3D11_SAMPLER_DESC sd = {};
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    GFX_THROW_INFO(getDevice(gfx)->CreateSamplerState(&sd, &m_sampler));
}


void mage::Sampler::bind(mage::Graphics& gfx) noexcept
{
    getContext(gfx)->PSSetSamplers(0, 1, m_sampler.GetAddressOf());
}
