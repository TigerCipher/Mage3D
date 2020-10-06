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
 * File Name: Mesh.h
 * Date File Created: 10/5/2020 at 4:42 PM
 * Author: Matt
 */
#pragma once

#include "Renderable.h"

class Mesh : public Renderable<Mesh>
{
public:
	Mesh(Graphics& gfx, list<UniquePtr<Bindable> > binds);


	void render(Graphics& gfx, mat4f accumulatedTransform) const noexcept(!MAGE_DEBUG);

	[[nodiscard]] mat4f getTransformMatrix() const noexcept override
	{
		return dx::XMLoadFloat4x4(&m_transform);
	}

private:
	mutable mat4x4 m_transform{ };
};

