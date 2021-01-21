// /*
//  * Mage3DX
//  * Copyright (C) 2021 Blue Moon Development. All rights reserved.
//  * This program is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  * GNU General Public License for more details.
//  * You should have received a copy of the GNU General Public License
//  * along with this program. If not, see <http://www.gnu.org/licenses/>.
//  *
//  * Contact: team@bluemoondev.org
//  *
//  * File Name: ModelException.h
//  * Date File Created: 01/20/2021 at 4:56 PM
//  * Author: Matt
//  */



#pragma once

#include "MageException.h"

#define MODEL_THROW_EXCEPTION(x) throw ModelException(__LINE__, __FILE__, x)

class ModelException : public MageException
{
public:
	ModelException(int line, const char* file, std::string info) noexcept :
		MageException(line, file),
		mInfo(std::move(info)) {}
	const char* what() const noexcept override;
	const char* getType() const noexcept override;
	const std::string& getInfo() const noexcept { return mInfo; }
protected:
private:

	std::string mInfo;
};
