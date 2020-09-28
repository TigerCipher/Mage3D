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
 * File Name: SolidSphere.h
 * Date File Created: 9/26/2020 at 4:14 PM
 * Author: Matt
 */
#pragma once
#include "Renderable.h"

namespace mage
{
	class SolidSphere : public Renderable<SolidSphere>
	{
	public:
		SolidSphere(Graphics& gfx, float radius);
		void update(float delta) noexcept override { }
		mat4f getTransformMatrix() const noexcept override;
		void setPosition(vec3f pos) noexcept { m_position = pos; }
	private:
		vec3f m_position = { 1.0f, 1.0f, 1.0f };
	};
}

