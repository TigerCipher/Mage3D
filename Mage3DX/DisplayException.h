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
 * File Name: DisplayException.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "MageException.h"

#define DISPLAY_EXCEPTION(hr)         DisplayException(__LINE__, __FILE__, hr)
#define DISPLAY_LAST_EXCEPTION()      DisplayException(__LINE__, __FILE__, GetLastError())
#define DISPLAY_NO_GFX_EXCEPTION()    NoGraphicsException(__LINE__, __FILE__)


class DisplayException : public MageException
{
public:
	DisplayException(const int line, const char* file, HRESULT hr) : MageException(line, file),
	                                                                 mResult(hr) { }
	const char* what() const noexcept override;
	HRESULT getError() const noexcept { return mResult; }
	std::string getErrorString() const noexcept;
private:
	HRESULT mResult;
};

class NoGraphicsException : public MageException
{
public:
	NoGraphicsException(const int line, const char* file) : MageException(line, file) { }
};
