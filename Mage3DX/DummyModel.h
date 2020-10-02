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
 * File Name: DummyModel.h
 * Date File Created: 10/2/2020 at 4:06 PM
 * Author: Matt
 */
#pragma once
#include "DummyObject.h"


namespace mage
{
	class DummyModel : public DummyObject<DummyModel>
	{
	public:
		DummyModel(Graphics& gfx, std::mt19937& rng,
		           std::uniform_real_distribution<float>& adist,
		           std::uniform_real_distribution<float>& ddist,
		           std::uniform_real_distribution<float>& odist,
		           std::uniform_real_distribution<float>& rdist,
		           DirectX::XMFLOAT3 material,
		           float scale);
	};
}

