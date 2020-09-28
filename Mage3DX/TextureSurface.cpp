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
 * File Name: texturesurface.cpp
 * Date File Created: 9/23/2020 at 4:07 PM
 * Author: Matt
 */

#include "TextureSurface.h"
#include "TextureException.h"

#define FULL_WINTARD

#include "winwrapper.h"

namespace Gdiplus
{
    using std::max;
    using std::min;
}// namespace Gdiplus
#include <gdiplus.h>

mage::TextureSurface& mage::TextureSurface::operator=(mage::TextureSurface&& src) noexcept
{
    m_width = src.m_width;
    m_height = src.m_height;
    m_buffer = std::move(src.m_buffer);
    src.m_buffer = nullptr;
    return *this;
}

void mage::TextureSurface::clear(mage::Color fill) noexcept
{
    memset(m_buffer.get(), fill.dword, m_width * m_height * sizeof(Color));
}

void mage::TextureSurface::setPixel(uint x, uint y, mage::Color col) noexcept(!MAGE_DEBUG)
{
    assert(x >= 0 && y >= 0 && x < m_width && y < m_height);
    m_buffer[ y * m_width + x ] = col;
}

mage::Color mage::TextureSurface::getPixel(uint x, uint y) const noexcept(!MAGE_DEBUG)
{
    assert(x >= 0 && y >= 0 && x < m_width && y < m_height);
    return m_buffer[ y * m_width + x ];
}

void mage::TextureSurface::save(const std::string& fileName) const
{
    auto getEncoder = [ &fileName ](const WCHAR* format, CLSID* clsid) -> void
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

        codecInfo = (Gdiplus::ImageCodecInfo*) (malloc(size));

        if (!codecInfo)
        {
            throw TextureException(
                    __LINE__, __FILE__,
                    fmt::format(
                            "Could not save texture surface to [{}]; Failed to get encoder; Could not allocate memory",
                            fileName));
        }

        Gdiplus::GetImageEncoders(num, size, codecInfo);
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
    getEncoder(L"ibmp", &bmp);

    wchar_t wideName[512];
    mbstowcs_s(nullptr, wideName, fileName.c_str(), _TRUNCATE);

    Gdiplus::Bitmap bitmap(m_width, m_height, m_width * sizeof(Color), PixelFormat32bppARGB, (BYTE*) m_buffer.get());
    if (bitmap.Save(wideName, &bmp, nullptr) != Gdiplus::Status::Ok)
    { throw TextureException(__LINE__, __FILE__, fmt::format("Could not save texture surface to [{}]", fileName)); }
}

void mage::TextureSurface::copy(const mage::TextureSurface& src) noexcept(!MAGE_DEBUG)
{
    assert(m_width == src.m_width);
    assert(m_height == src.m_height);
    memcpy(m_buffer.get(), src.m_buffer.get(), m_width * m_height * sizeof(Color));
}

mage::TextureSurface mage::TextureSurface::loadFromFile(const std::string& fileName)
{
    uint width = 0;
    uint height = 0;
    Scope<Color[]> buf;

    wchar_t wideName[512];
    mbstowcs_s(nullptr, wideName, fileName.c_str(), _TRUNCATE);
    Gdiplus::Bitmap bitmap(wideName);
    if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
    {
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
        }
    }

    return TextureSurface(width, height, std::move(buf));
}

ULONG_PTR mage::GDIPlusManager::s_token = 0;

void mage::GDIPlusManager::start() noexcept
{
    Gdiplus::GdiplusStartupInput input;
    input.GdiplusVersion = 1;
    input.DebugEventCallback = nullptr;
    input.SuppressBackgroundThread = false;
    Gdiplus::GdiplusStartup(&s_token, &input, nullptr);
}
void mage::GDIPlusManager::stop() noexcept
{
    Gdiplus::GdiplusShutdown(s_token);
}
