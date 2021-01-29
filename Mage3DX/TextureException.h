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
 * File Name: TextureException.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "MageException.h"
#include "DisplayException.h"

class TextureException : public MageException
{
public:
	TextureException(const int line, const char* file, std::string info, std::optional<HRESULT> hr = {}) noexcept :
		MageException(line, file),
		mInfo(std::move(info))
	{
		if (hr)
			mInfo = fmt::format("[Error Code] {}\n{}", ExceptionHelper::translateError(*hr), mInfo);
	}
	const char* what() const noexcept override;
	const std::string& getInfo() const noexcept { return mInfo; }

private:
	std::string mInfo;
};



