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
 * File Name: Bindable.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "Graphics.h"


class Bindable
{
public:
	virtual ~Bindable() = default;
	virtual void bind(Graphics& gfx) noexcept = 0;

	virtual std::string getUID() const noexcept
	{
		return typeid(*this).name();
	}

protected:
	static ID3D11DeviceContext* getContext(Graphics& gfx) noexcept;
	static ID3D11Device* getDevice(Graphics& gfx) noexcept;
	static DebugInfo& getDebugInfo(Graphics& gfx) noexcept(MAGE_DEBUG);
};
