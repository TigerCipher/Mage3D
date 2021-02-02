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

/// <summary>
/// Turns a vector of strings into a specified char separated single string
/// </summary>
/// <param name="strList">The vector of strings</param>
/// <param name="sep">The character to separate each string element by</param>
/// <returns>A specified char separated string of the values from the vector</returns>
extern std::string from_list(const list<std::string>& strList, const char sep = '\n');

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


/// <summary>
/// Centers the given string into a string of <code>targetSize</code> length
/// </summary>
/// <param name="original">The string to center</param>
/// <param name="targetSize">The desired length/width of the new string</param>
/// <param name="fill">The character to fill before and after the original string, default is a space</param>
/// <returns>The original string, centered</returns>
extern std::string centered(const std::string& original, const int targetSize, const char fill = ' ');
