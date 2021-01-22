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

class Texture
{
public:
	Texture(uint width, uint height, uint pitch) noexcept :
		mWidth(width),
		mHeight(height),
		mBuffer(createScope<Color[]>(pitch * height))
	{ }
	Texture(uint width, uint height) noexcept : Texture(width, height, width) { }

	virtual ~Texture() = default;
	Texture(const Texture& rhs) = delete;
	Texture& operator=(const Texture& rhs) = delete;
	Texture(Texture&& src) noexcept :
		mWidth(src.mWidth),
		mHeight(src.mHeight),
		mBuffer(std::move(src.mBuffer))
	{ }
	Texture& operator=(Texture&& src) noexcept;

	void clear(Color fill) const noexcept;
	void setPixel(uint x, uint y, Color col) const noexcept(!MAGE_DEBUG);
	[[nodiscard]] Color getPixel(uint x, uint y) const noexcept(!MAGE_DEBUG);

	Color* getBuffer() noexcept { return mBuffer.get(); }
	[[nodiscard]] const Color* getBuffer() const noexcept { return mBuffer.get(); }

	[[nodiscard]] uint getWidth() const noexcept { return mWidth; }
	[[nodiscard]] uint getHeight() const noexcept { return mHeight; }

	void save(const std::string& fileName) const;
	void copy(const Texture& src) const noexcept(!MAGE_DEBUG);

	static Texture loadFromFile(const std::string& fileName);

protected:
private:
	Texture(uint width, uint height, UniquePtr<Color[]> buffer) noexcept :
		mWidth(width),
		mHeight(height),
		mBuffer(std::move(buffer))
	{ }

	UniquePtr<Color[]> mBuffer;
	uint mWidth;
	uint mHeight;
};


