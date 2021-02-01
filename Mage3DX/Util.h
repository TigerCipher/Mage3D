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
 * File Name: Util.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

extern void from_list(std::string& str, const list<std::string>& strList);

/// <summary>
/// Converts a narrow std::string to a wide std::wstring.
/// Maximum length supported is 512 characters (including null terminating character)
/// </summary>
/// <param name="str">The narrow string to be converted</param>
/// <returns>A wide string</returns>
extern std::wstring to_wide(const std::string& str);



/// <summary>
/// Converts a wide std::wstring to a narrow std::string.
/// Maximum length supported is 512 characters (including null terminating character)
/// </summary>
/// <param name="str">The wide string to be converted</param>
/// <returns>A narrow string</returns>
extern std::string to_narrow(const std::wstring& str);



extern list<std::string> tokenize_quoted(const std::string& input);