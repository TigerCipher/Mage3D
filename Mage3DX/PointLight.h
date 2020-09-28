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
 * File Name: PointLight.h
 * Date File Created: 9/26/2020 at 4:43 PM
 * Author: Matt
 */
#pragma once

#include "graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffer.h"

namespace mage
{
	class PointLight
	{
	public:
		explicit PointLight(Graphics& gfx, float radius = 0.5f);

		void render(Graphics& gfx) const noexcept(!MAGE_DEBUG);
		void bind(Graphics& gfx, mat4f view) const noexcept;
		
		void spawnControlWindow() noexcept;
		void reset() noexcept;

	private:
		struct PointLightCBuf
		{
			alignas(16) vec3f pos;
			alignas(16) vec3f diffuseColor;
			alignas(16) vec3f ambient;

			float diffuseIntensity;
			
			float attConst;
			float attLin;
			float attQuad;
		};

		PointLightCBuf m_cbuf;
		mutable SolidSphere m_mesh;
		mutable PixelConstantBuffer<PointLightCBuf> m_buffer;
	};
}

