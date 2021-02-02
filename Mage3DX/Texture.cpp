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

#include "Texture.h"
#include "TextureException.h"

#include <filesystem>

Texture::Texture(const uint width, const uint height)
{
	HRESULT hr = mScratch.Initialize2D(TEXTURE_FORMAT, width, height, 1u, 1u);

	if(FAILED(hr))
	{
		throw TextureException(__LINE__, __FILE__, "Failed to initialize DirectX ScratchImage", hr);
	}
}

void Texture::clear(const Color fillValue) const noexcept
{
	const auto w = getWidth();
	const auto h = getHeight();
	auto& img = *mScratch.GetImage(0, 0, 0);

	for(size_t y = 0; y < h; y++)
	{
		auto* row = reinterpret_cast<Color*>(img.pixels + img.rowPitch * y);
		std::fill(row, row + img.width, fillValue);
	}
}

void Texture::setPixel(const uint x, const uint y, const Color col) const noexcept(!MAGE_DEBUG)
{
	assert(x >= 0 && y >= 0 && x < getWidth() && y < getHeight());
	auto& img = *mScratch.GetImage(0, 0, 0);
	reinterpret_cast<Color*>(&img.pixels[y * img.rowPitch])[x] = col;
}

Color Texture::getPixel(const uint x, const uint y) const noexcept(!MAGE_DEBUG)
{
	assert(x >= 0 && y >= 0 && x < getWidth() && y < getHeight());
	auto& img = *mScratch.GetImage(0, 0, 0);
	return reinterpret_cast<Color*>(&img.pixels[y * img.rowPitch])[x];
}

void Texture::save(const std::string& fileName) const
{
	const std::filesystem::path path = fileName;
	const auto ext = path.extension().string();
	auto getEncoder = [ ext ] (const std::string& file)
		{
		if (ext == ".png") return dx::WIC_CODEC_PNG;
		if (ext == ".bmp") return dx::WIC_CODEC_BMP;
		if (ext == ".gif") return dx::WIC_CODEC_GIF;
		if (ext == ".jpg") return dx::WIC_CODEC_JPEG;
		if (ext == ".tif") return dx::WIC_CODEC_TIFF;
		if (ext == ".ico") return dx::WIC_CODEC_ICO;

		throw TextureException(__LINE__, __FILE__, fmt::format("Texture format not supported [{}]", file));
		};

	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, fileName.c_str(), _TRUNCATE);
	
	HRESULT hr;
	if (ext == ".dds")
		hr = dx::SaveToDDSFile(*mScratch.GetImage(0, 0, 0), dx::DDS_FLAGS_NONE, wideName);
	else
		hr = dx::SaveToWICFile(*mScratch.GetImage(0, 0, 0),
			dx::WIC_FLAGS_NONE, dx::GetWICCodec(getEncoder(fileName)), wideName);

	if(FAILED(hr))
	{
		throw TextureException(__LINE__, __FILE__, fmt::format("Failed to save texture [{}]", fileName), hr);
	}
}


Texture Texture::loadFromFile(const std::string& fileName)
{
	const std::filesystem::path path = fileName;
	const auto ext = path.extension().string();
	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, fileName.c_str(), _TRUNCATE);

	dx::ScratchImage scratch;
	HRESULT hr;

	if (ext == ".dds")
		hr = dx::LoadFromDDSFile(wideName, dx::DDS_FLAGS_NONE, nullptr, scratch);
	else if (ext == ".png" || ext == ".jpg" || ext == ".bmp" || ext == ".tif" || ext == ".gif")
		hr = dx::LoadFromWICFile(wideName, dx::WIC_FLAGS_NONE, nullptr, scratch);
	else
		throw TextureException(__LINE__, __FILE__,
			fmt::format("Textures of format [{}] are not currently supported", ext));

	if(FAILED(hr))
	{
		LOG_WARN("Could not load texture [{}]", fileName);
		throw TextureException(__LINE__, __FILE__, fmt::format("Could not load texture [{}]", fileName), hr);
	}

	if(scratch.GetImage(0, 0, 0)->format != TEXTURE_FORMAT)
	{
		LOG_DEBUG("Texture [{}] is of format {}", fileName, scratch.GetImage(0, 0, 0)->format);
		dx::ScratchImage converted;
		hr = dx::Convert(*scratch.GetImage(0, 0, 0), TEXTURE_FORMAT,
			dx::TEX_FILTER_DEFAULT, dx::TEX_THRESHOLD_DEFAULT, converted);
		if(FAILED(hr))
		{
			throw TextureException(__LINE__, __FILE__,
				fmt::format("Failed to convert texture format [{}]", fileName), hr);
		}
		LOG_TRACE("Loaded and converted texture [{}]", fileName);
		return Texture(std::move(converted));
	}

	LOG_TRACE("Loaded texture [{}]", fileName);
	return Texture(std::move(scratch));
}

