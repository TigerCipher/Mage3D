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
 * File Name: texturesurface.h
 * Date File Created: 9/23/2020 at 4:07 PM
 * Author: Matt
 */

#pragma once

#include "Color.h"

namespace mage
{
    class TextureSurface
    {
    public:
        TextureSurface(uint width, uint height, uint pitch) noexcept :
            m_width(width), m_height(height), m_buffer(createScope<Color[]>(pitch * height))
        { }
        TextureSurface(uint width, uint height) noexcept : TextureSurface(width, height, width) { }

        virtual ~TextureSurface() = default;
        TextureSurface(const TextureSurface& rhs) = delete;
        TextureSurface& operator=(const TextureSurface& rhs) = delete;
        TextureSurface(TextureSurface&& src) noexcept :
            m_width(src.m_width), m_height(src.m_height), m_buffer(std::move(src.m_buffer))
        { }
        TextureSurface& operator=(TextureSurface&& src) noexcept;

        void clear(Color fill) noexcept;
        void setPixel(uint x, uint y, Color col) noexcept(!MAGE_DEBUG);
        [[nodiscard]] Color getPixel(uint x, uint y) const noexcept(!MAGE_DEBUG);

        Color* getBuffer() noexcept { return m_buffer.get(); }
        [[nodiscard]] const Color* getBuffer() const noexcept { return m_buffer.get(); }

        [[nodiscard]] uint getWidth() const noexcept { return m_width; }
        [[nodiscard]] uint getHeight() const noexcept { return m_height; }

        void save(const std::string& fileName) const;
        void copy(const TextureSurface& src) noexcept(!MAGE_DEBUG);

        static TextureSurface loadFromFile(const std::string& fileName);

    protected:
    private:
        TextureSurface(uint width, uint height, UniquePtr<Color[]> buffer) noexcept :
            m_width(width), m_height(height), m_buffer(std::move(buffer))
        { }

        UniquePtr<Color[]> m_buffer;
        uint m_width;
        uint m_height;
    };

    // TODO: Move to own file
    class GDIPlusManager
    {
    public:
        static void start() noexcept;
        static void stop() noexcept;

    private:
        static ULONG_PTR s_token;
    };

}// namespace mage

