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
 * File Name: Util.cpp
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */

#include "Util.h"


void from_list(std::string& str, const list<std::string>& strList)
{
	for(const auto& s : strList)
	{
		str += s;
		str.push_back('\n');
	}

	if(!str.empty())
		str.pop_back();
}

std::wstring to_wide(const std::string& str)
{
	wchar_t buf[512];
	mbstowcs_s(nullptr, buf, str.c_str(), _TRUNCATE);
	return buf;
}


std::string to_narrow(const std::wstring& str)
{
	char buf[512];
	wcstombs_s(nullptr, buf, str.c_str(), _TRUNCATE);
	return buf;
}

list<std::string> tokenize_quoted(const std::string& input)
{
	std::istringstream iss;
	iss.str(input);
	list<std::string> tokens;
	std::string token;

	while(iss >> std::quoted(token))
	{
		tokens.push_back(std::move(token));
	}

	return tokens;
}
