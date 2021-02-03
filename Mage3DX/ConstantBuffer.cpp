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
 * File Name: ConstantBuffer.cpp
 * Date File Created: 2/2/2021 at 8:18 PM
 * Author: Matt
 */

#include "ConstantBuffer.h"

void PixelConstantBufferEx::update(Graphics& gfx, const dcb::Buffer& buf) const
{
	DEBUG_INFO(gfx);

	D3D11_MAPPED_SUBRESOURCE ms;
	GFX_THROW_INFO(getContext(gfx)->Map(mCBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms));

	memcpy(ms.pData, buf.getData(), buf.getSizeInBytes());
	getContext(gfx)->Unmap(mCBuf.Get(), 0);
}

void PixelConstantBufferEx::bind(Graphics& gfx) noexcept
{
	getContext(gfx)->PSSetConstantBuffers(mSlot, 1, mCBuf.GetAddressOf());
}

PixelConstantBufferEx::PixelConstantBufferEx(Graphics& gfx, SharedPtr<dcb::LayoutElement> lay, uint slot,
	const dcb::Buffer* buf) :
	mSlot(slot),
	mLayout(std::move(lay))
{
	DEBUG_INFO(gfx);

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = static_cast<UINT>(mLayout->getSizeInBytes());

	if(buf)
	{
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = buf->getData();
		GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&cbd, &sd, &mCBuf));
	}else
	{
		GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&cbd, nullptr, &mCBuf));
	}
}
