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
 * File Name: ConstantBuffer.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "GraphicsException.h"
#include "Bindable.h"
#include "BindableManger.h"
#include "DynamicConstantBuffer.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	explicit ConstantBuffer(Graphics& gfx, UINT slot = 0) : mSlot(slot)
	{
		DEBUG_INFO(gfx);
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(C);
		desc.StructureByteStride = 0;
		GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&desc, nullptr, &mBuffer));
	}

	ConstantBuffer(Graphics& gfx, const C& consts, UINT slot = 0) : mSlot(slot)
	{
		DEBUG_INFO(gfx);
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(consts);
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subData = { };
		subData.pSysMem = &consts;

		GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&desc, &subData, &mBuffer));
	}

	void update(Graphics& gfx, const C& consts)
	{
		DEBUG_INFO(gfx);

		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO(getContext(gfx)->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &consts, sizeof(consts));
		getContext(gfx)->Unmap(mBuffer.Get(), 0);
	}


protected:
	COMptr<ID3D11Buffer> mBuffer;
	UINT mSlot;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
using ConstantBuffer<C>::mBuffer;
using ConstantBuffer<C>::mSlot;
//using Bindable::getContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;

	void bind(Graphics& gfx) noexcept override
	{
		// For some reason ReSharper (but not Visual C++) was thinking getContext was private and not protected
		// but super:: works so whatever
		__super::getContext(gfx)->VSSetConstantBuffers(mSlot, 1, mBuffer.GetAddressOf());
	}

	static SharedPtr<VertexConstantBuffer> resolve(Graphics& gfx, const C& consts, uint slot = 0)
	{
		return BindableManger::resolve<VertexConstantBuffer>(gfx, consts, slot);
	}

	static SharedPtr<VertexConstantBuffer> resolve(Graphics& gfx, uint slot = 0)
	{
		return BindableManger::resolve<VertexConstantBuffer>(gfx, slot);
	}

	static std::string generateUID(const C& consts, uint slot)
	{
		return generateUID(slot);
	}

	static std::string generateUID(uint slot = 0)
	{
		using namespace std::string_literals;
		return typeid(VertexConstantBuffer).name() + "#"s + std::to_string(slot);
	}

	std::string getUID() const noexcept override
	{
		return generateUID(mSlot);
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
using ConstantBuffer<C>::mBuffer;
using ConstantBuffer<C>::mSlot;
//using Bindable::getContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;

	void bind(Graphics& gfx) noexcept override
	{
		__super::getContext(gfx)->PSSetConstantBuffers(mSlot, 1, mBuffer.GetAddressOf());
	}

	static SharedPtr<PixelConstantBuffer> resolve(Graphics& gfx, const C& consts, uint slot = 0)
	{
		return BindableManger::resolve<PixelConstantBuffer>(gfx, consts, slot);
	}

	static SharedPtr<PixelConstantBuffer> resolve(Graphics& gfx, uint slot = 0)
	{
		return BindableManger::resolve<PixelConstantBuffer>(gfx, slot);
	}

	static std::string generateUID(const C& consts, uint slot)
	{
		return generateUID(slot);
	}

	static std::string generateUID(uint slot = 0)
	{
		using namespace std::string_literals;
		return typeid(PixelConstantBuffer).name() + "#"s + std::to_string(slot);
	}

	std::string getUID() const noexcept override
	{
		return generateUID(mSlot);
	}
};


class PixelConstantBufferEx : public Bindable
{
public:

	PixelConstantBufferEx(Graphics& gfx, SharedPtr<dcb::LayoutElement> lay, const uint slot) :
		PixelConstantBufferEx(gfx, std::move(lay), slot, nullptr) { }

	PixelConstantBufferEx(Graphics& gfx, const dcb::Buffer& buf, const uint slot) :
		PixelConstantBufferEx(gfx, buf.cloneLayout(), slot, &buf) { }

	void update(Graphics& gfx, const dcb::Buffer& buf) const;

	void bind(Graphics& gfx) noexcept override;

	[[nodiscard]] const dcb::LayoutElement& getLayout() const noexcept
	{
		return *mLayout;
	}

private:
	PixelConstantBufferEx(Graphics& gfx, SharedPtr<dcb::LayoutElement> lay, uint slot, const dcb::Buffer* buf);
	
	uint mSlot;
	COMptr<ID3D11Buffer> mCBuf;
	SharedPtr<dcb::LayoutElement> mLayout;
};