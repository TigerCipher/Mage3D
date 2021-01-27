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
 * File Name: Texture.cpp
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "Texture.h"
#include "TextureException.h"

#define FULL_WINTARD

#include "winwrapper.h"

namespace Gdiplus
{
	using std::max;
	using std::min;
}// namespace Gdiplus
#include <gdiplus.h>

Texture& Texture::operator=(Texture&& src) noexcept
{
	mWidth = src.mWidth;
	mHeight = src.mHeight;
	mBuffer = std::move(src.mBuffer);
	mAlphaLoaded = src.mAlphaLoaded;
	src.mBuffer = nullptr;
	return *this;
}

void Texture::clear(const Color fill) const noexcept
{
	memset(mBuffer.get(), fill.dword, mWidth * mHeight * sizeof(Color));
}

void Texture::setPixel(const uint x, const uint y, const Color col) const noexcept(!MAGE_DEBUG)
{
	assert(x >= 0 && y >= 0 && x < mWidth && y < mHeight);
	mBuffer[ y * mWidth + x ] = col;
}

Color Texture::getPixel(const uint x, const uint y) const noexcept(!MAGE_DEBUG)
{
	assert(x >= 0 && y >= 0 && x < mWidth && y < mHeight);
	return mBuffer[ y * mWidth + x ];
}

void Texture::save(const std::string& fileName) const
{
	auto getEncoder = [ &fileName ] (const WCHAR* format, CLSID* clsid) -> void
		{
			UINT num = 0;
			UINT size = 0;

			Gdiplus::ImageCodecInfo* codecInfo = nullptr;
			Gdiplus::GetImageEncodersSize(&num, &size);
			if (size == 0)
			{
				throw TextureException(
					__LINE__, __FILE__,
					fmt::format("Could not save texture surface to [{}]; Failed to get encoder. Size = 0", fileName));
			}

			codecInfo = static_cast<Gdiplus::ImageCodecInfo*>(malloc(size));

			if (!codecInfo)
			{
				throw TextureException(
					__LINE__, __FILE__,
					fmt::format(
						"Could not save texture surface to [{}]; Failed to get encoder; Could not allocate memory",
						fileName));
			}

			Gdiplus::GetImageEncoders(num, size, codecInfo);
			LOG_DEBUG("Num {} Size {}", num, size);
			for (UINT i = 0; i < num; i++)
			{
				if (wcscmp(codecInfo[ i ].MimeType, format) == 0)
				{
					*clsid = codecInfo[ i ].Clsid;
					free(codecInfo);
					return;
				}
			}

			free(codecInfo);
			throw TextureException(__LINE__, __FILE__,
				fmt::format("Could not save texture surface to [{}]; Failed to get encoder", fileName));
		};

	CLSID bmp;
	getEncoder(L"image/bmp", &bmp);

	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, fileName.c_str(), _TRUNCATE);

	Gdiplus::Bitmap bitmap(mWidth, mHeight, mWidth * sizeof(Color), PixelFormat32bppARGB, reinterpret_cast<BYTE*>(mBuffer.get()));
	if (bitmap.Save(wideName, &bmp, nullptr) != Gdiplus::Status::Ok)
	{
		throw TextureException(__LINE__, __FILE__, fmt::format("Could not save texture surface to [{}]", fileName));
	}
}

void Texture::copy(const Texture& src) const noexcept(!MAGE_DEBUG)
{
	assert(mWidth == src.mWidth);
	assert(mHeight == src.mHeight);
	memcpy(mBuffer.get(), src.mBuffer.get(), mWidth * mHeight * sizeof(Color));
}

Texture Texture::loadFromFile(const std::string& fileName)
{
	uint width = 0;
	uint height = 0;
	Scope<Color[]> buf;
	bool alphaLoaded = false;

	{
		wchar_t wideName[512];
		mbstowcs_s(nullptr, wideName, fileName.c_str(), _TRUNCATE);
		Gdiplus::Bitmap bitmap(wideName);
		Gdiplus::Status errStatus = bitmap.GetLastStatus();
		if (errStatus != Gdiplus::Status::Ok)
		{
			LOG_ERROR("Failed to load texture [{}]. Status {}", fileName, errStatus);
			throw TextureException(__LINE__, __FILE__, fmt::format("Failed to load texture [{}]", fileName));
		}
		width = bitmap.GetWidth();
		height = bitmap.GetHeight();
		buf = createScope<Color[]>(width * height);

		for (uint y = 0; y < height; y++)
		{
			for (uint x = 0; x < width; x++)
			{
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				buf[ y * width + x ] = c.GetValue();
				if (c.GetAlpha() != 255)
					alphaLoaded = true;
			}
		}
	}

	LOG_TRACE("Loaded texture [{}]", fileName);
	return Texture(width, height, std::move(buf), alphaLoaded);
}

