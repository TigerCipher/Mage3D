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
 * File Name: Settings.cpp
 * Date File Created: 1/28/2021 at 5:27 PM
 * Author: Matt
 */
#include "Settings.h"

mINI::INIStructure Settings::sIni;

void Settings::load(const std::string& path)
{
	const mINI::INIFile file(path);
	file.read(sIni);
}

void Settings::save(const std::string& path)
{
	const mINI::INIFile file(path);
	file.write(sIni);
}

float Settings::getFloat(const std::string& category, const std::string& name)
{
	return std::stof(sIni[category][name]);
}

int Settings::getInt(const std::string& category, const std::string& name)
{
	return std::stoi(sIni[category][name]);
}

bool Settings::getBool(const std::string& category, const std::string& name)
{
	return std::stoi(sIni[category][name]) == 1 ? true : false;
}

std::string Settings::getString(const std::string& category, const std::string& name)
{
	return sIni[category][name];
}

float Settings::getFloat(const std::string& category, const std::string& name, const list<float>& options)
{
	const auto ret = getFloat(category, name);
	for (auto option : options)
	{
		if (ret == option) return ret;
	}

	return options[0];
}


int Settings::getInt(const std::string& category, const std::string& name, const list<int>& options)
{
	const auto ret = getInt(category, name);
	for (auto option : options)
	{
		if (ret == option) return ret;
	}

	return options[0];
}

void Settings::setFloat(const std::string& category, const std::string& name, const float value)
{
	sIni[category][name] = std::to_string(value);
}

void Settings::setInt(const std::string& category, const std::string& name, const int value)
{
	sIni[category][name] = std::to_string(value);
}

void Settings::setBool(const std::string& category, const std::string& name, const bool value)
{
	sIni[category][name] = value ? "1" : "0";
}

void Settings::setString(const std::string& category, const std::string& name, const std::string& value)
{
	sIni[category][name] = value;
}
