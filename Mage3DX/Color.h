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
 * File Name: Color.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once



class Color
{
public:
	constexpr Color() noexcept : dword() { }
	constexpr explicit Color(uint dword_) noexcept : dword(dword_) { }
	constexpr Color(ubyte a, ubyte r, ubyte g, ubyte b) noexcept : dword((a << 24) | (r << 16) | (g << 8) | b) { }
	constexpr Color(ubyte r, ubyte g, ubyte b) noexcept : dword((r << 16) | (g << 8) | b) { }
	constexpr Color(Color col, ubyte a) noexcept : Color((a << 24) | col.dword) { }
	//constexpr Color(const Color& col) noexcept = default;

	//Color& operator=(Color col) noexcept
	//{
	//	dword = col.dword;
	//	return *this;
	//}

	Color& operator=(ulong col) noexcept
	{
		dword = col;
		return *this;
	}

	void setAlpha(ubyte a) noexcept { dword = (dword & 0xffffff) | (a << 24); }
	void setRed(ubyte r) noexcept { dword = (dword & 0xff00ffff) | (r << 16); }
	void setGreen(ubyte g) noexcept { dword = (dword & 0xffff00ff) | (g << 8); }
	void setBlue(ubyte b) noexcept { dword = (dword & 0xffffff00) | b; }

	[[nodiscard]] constexpr ubyte getAlpha() const noexcept { return dword >> 24; }
	[[nodiscard]] constexpr ubyte getRed() const noexcept { return (dword >> 16) & 0xff; }
	[[nodiscard]] constexpr ubyte getGreen() const noexcept { return (dword >> 8) & 0xff; }
	[[nodiscard]] constexpr ubyte getBlue() const noexcept { return dword & 0xff; }

	uint dword;
};

