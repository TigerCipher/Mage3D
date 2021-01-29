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
 * File Name: Settings.h
 * Date File Created: 1/28/2021 at 5:27 PM
 * Author: Matt
 */
#pragma once

#include <mini/ini.h>

class Settings
{
public:
	static void load(const std::string& path);
	static void save(const std::string& path);
	
	static float getFloat(const std::string& category, const std::string& name);
	static int getInt(const std::string& category, const std::string& name);
	static bool getBool(const std::string& category, const std::string& name);
	static std::string getString(const std::string& category, const std::string& name);

	static float getFloat(const std::string& category, const std::string& name, const list<float>& options);
	static int getInt(const std::string& category, const std::string& name, const list<int>& options);

	static void setFloat(const std::string& category, const std::string& name, float value);
	static void setInt(const std::string& category, const std::string& name, int value);
	static void setBool(const std::string& category, const std::string& name, bool value);
	static void setString(const std::string& category, const std::string& name, const std::string& value);

private:
	static mINI::INIStructure sIni;
};
