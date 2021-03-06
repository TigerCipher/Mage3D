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
 * File Name: Callstack.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

struct Entry
{
	std::string file;
	uint line;
	std::string function;

	Entry() : line(0) { }

	[[nodiscard]] std::string asString() const
	{
		return fmt::format("{} ({}): {}", file, line, function);
	}
};

class Callstack
{
public:
	explicit Callstack(int numDiscard = 0);
	virtual ~Callstack() noexcept = default;

	[[nodiscard]] std::string asString() const
	{
		//return "Stacktrace code is broken";
		std::ostringstream oss;
		for(const auto & i : mStack)
		{
			oss << i.asString() << "\n";
		}
		
		return oss.str();
	}

private:
	list<Entry> mStack;
};
