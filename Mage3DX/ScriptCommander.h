/*
 * Mage3DX
 * Copyright (C) 2021 Blue Moon Development. All rights reserved.
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
 * File Name: ScriptCommander.h
 * Date File Created: 2/1/2021 at 2:46 PM
 * Author: Matt
 */


#pragma once

#include <utility>

#include "MageException.h"

class ScriptCommander
{
public:
	ScriptCommander(const list <std::string>& args);


	class ParseException : public MageException
	{
	public:
		ParseException(int line, const char* file, std::string cmd = "", std::string msg = "") :
			MageException(line, file),
			mCommand(std::move(cmd)),
			mMessage(std::move(msg)) {}
		char const* what() const noexcept override;
	private:
		std::string mCommand;
		std::string mMessage;
	};

	class SuccessException : public MageException
	{
	public:
		SuccessException(const std::string& content) noexcept :
			MageException(0, "# Command Parser Success #"),
			mContent(content) {}
		char const* what() const noexcept override;
	private:
		std::string mContent;
	};
protected:
private:
};
