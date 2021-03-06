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
 * File Name: Topology.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "Bindable.h"


class Topology : public Bindable
{
public:
	Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type) : mType(type) { }
	void bind(Graphics& gfx) noexcept override;
	static SharedPtr<Topology> resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	static std::string generateUID(D3D11_PRIMITIVE_TOPOLOGY type);
	std::string getUID() const noexcept override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY mType;
};

