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
 * File Name: LayoutManager.h
 * Date File Created: 2/3/2021 at 3:14 PM
 * Author: Matt
 */

#pragma once

#include "DynamicConstantBuffer.h"

class LayoutManager
{
public:

	static bool has(const std::string& tag)
	{
		return get().has_(tag);
	}

	static dcb::Layout load(const std::string& tag)
	{
		return get().load_(tag);
	}

	static bool load(const std::string& tag, dcb::Layout& layout)
	{
		if (!get().has_(tag)) return false;
		layout = get().load_(tag);
		return true;
	}

	static void store(const std::string& tag, dcb::Layout& layout)
	{
		get().store_(tag, layout);
	}

private:

	static LayoutManager& get()
	{
		static LayoutManager mngr;
		return mngr;
	}

	bool has_(const std::string& tag) const
	{
		return mMap.find(tag) != mMap.end();
	}

	dcb::Layout load_(const std::string& tag) const
	{
		return { mMap.find(tag)->second };
	}

	void store_(const std::string& tag, dcb::Layout& layout)
	{
		auto r = mMap.insert({ tag, layout.finish() });
		assert(r.second);
	}


	std::unordered_map<std::string, SharedPtr<dcb::LayoutElement>> mMap;
};
