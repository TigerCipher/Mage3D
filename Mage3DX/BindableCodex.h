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
 * File Name: BindableCodex.h
 * Date File Created: 1/22/2021 at 4:35 PM
 * Author: Matt
 */
#pragma once

#include "Bindable.h"

class BindableCodex
{
public:

	template<class T, typename... Args>
	static SharedPtr<T> resolve(Graphics& gfx, Args&&... args) noexcept(!MAGE_DEBUG)
	{
		static_assert(std::is_base_of<Bindable, T>::value, "Can only resolve classes that inherit Bindable");
		return get().resolve_<T>(gfx, std::forward<Args>(args)...);
	}

private:
	template<class T, typename... Args>
	[[nodiscard]] SharedPtr<T> resolve_(Graphics& gfx, Args&&... args) noexcept(!MAGE_DEBUG)
	{
		const auto key = T::generateUID(std::forward<Args>(args)...);
		const auto i = mBinds.find(key);
		if(i == mBinds.end())
		{
			auto bind = createRef<T>(gfx, std::forward<Args>(args)...);
			//auto bind = std::make_shared<T>(gfx, std::forward<Args>(args)...);
			mBinds[key] = bind;
			return bind;
		}

		return std::static_pointer_cast<T>(i->second);
	}

	static BindableCodex& get()
	{
		static BindableCodex codex;
		return codex;
	}


	std::unordered_map<std::string, SharedPtr<Bindable>> mBinds;
};