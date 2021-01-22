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
 * File Name: texture.cpp
 * Date File Created: 9/23/2020 at 10:48 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "TextureData.h"
#include "GraphicsException.h"
#include "Texture.h"



TextureData::TextureData(Graphics& gfx, const Texture& surface, uint slot) :
	mSlot(slot)
{
	DEBUG_INFO(gfx);

	// create texture resource
	D3D11_TEXTURE2D_DESC textureDesc = { };
	textureDesc.Width = surface.getWidth();
	textureDesc.Height = surface.getHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sd = { };
	sd.pSysMem = surface.getBuffer();
	sd.SysMemPitch = surface.getWidth() * sizeof(Color);
	COMptr<ID3D11Texture2D> tex;
	GFX_THROW_INFO(getDevice(gfx)->CreateTexture2D(&textureDesc, &sd, &tex));

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GFX_THROW_INFO(getDevice(gfx)->CreateShaderResourceView(tex.Get(), &srvDesc, &mTextureView));
}

void TextureData::bind(Graphics& gfx) noexcept
{
	getContext(gfx)->PSSetShaderResources(mSlot, 1, mTextureView.GetAddressOf());
}
