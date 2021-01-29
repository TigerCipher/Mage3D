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
 * File Name: Texture.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "Color.h"

#include <DirectXTex.h>

namespace dx = DirectX;

class Texture
{
	friend class TextureProcessor;
public:

	enum TextureType
	{
		WIC, // png, jpg, bmp, tif, gif
		DDS
	};
	
	Texture(const uint width, const uint height);

	virtual ~Texture() = default;
	Texture(Texture&& src) noexcept = default;
	Texture& operator=(Texture&& src) noexcept = default;
	
	Texture(const Texture& rhs) = delete;
	Texture& operator=(const Texture& rhs) = delete;

	void clear(Color fill) const noexcept;
	void setPixel(uint x, uint y, Color col) const noexcept(!MAGE_DEBUG);
	[[nodiscard]] Color getPixel(uint x, uint y) const noexcept(!MAGE_DEBUG);

	Color* getBuffer() noexcept { return reinterpret_cast<Color*>(mScratch.GetPixels()); }
	[[nodiscard]] const Color* getBuffer() const noexcept { return const_cast<Texture*>(this)->getBuffer(); }

	[[nodiscard]] uint getWidth() const noexcept { return static_cast<uint>(mScratch.GetMetadata().width); }
	[[nodiscard]] uint getHeight() const noexcept { return static_cast<uint>(mScratch.GetMetadata().height); }

	void save(const std::string& fileName) const;

	[[nodiscard]] bool isAlphaLoaded() const noexcept { return !mScratch.IsAlphaAllOpaque(); }

	static Texture loadFromFile(const std::string& fileName);

	static constexpr DXGI_FORMAT TEXTURE_FORMAT = DXGI_FORMAT_B8G8R8A8_UNORM;
protected:
private:
	Texture(dx::ScratchImage scratch) noexcept : mScratch(std::move(scratch)) {}

	
	
	dx::ScratchImage mScratch{};
	
};


