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
 * File Name: topology.cpp
 * Date File Created: 9/20/2020 at 10:09 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "Topology.h"
#include "BindableCodex.h"

void Topology::bind(Graphics& gfx) noexcept
{
    getContext(gfx)->IASetPrimitiveTopology(mType);
}

SharedPtr<Topology> Topology::resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
{
	return BindableCodex::resolve<Topology>(gfx, type);
}

std::string Topology::generateUID(D3D11_PRIMITIVE_TOPOLOGY type)
{
	using namespace std::string_literals;
	return typeid(Topology).name() + "#"s + std::to_string(type);
}

std::string Topology::getUID() const noexcept
{
	return generateUID(mType);
}
