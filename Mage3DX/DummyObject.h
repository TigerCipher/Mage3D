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
 * File Name: DummyObject.h
 * Date File Created: 9/28/2020 at 2:37 AM
 * Author: Matt
 */
#pragma once
#include "renderable.h"


namespace mage
{
	template<class T>
	class DummyObject : public Renderable<T>
	{
	public:

		DummyObject(Graphics& gfx, std::mt19937& rng,
		            std::uniform_real_distribution<float>& adist,
		            std::uniform_real_distribution<float>& ddist,
		            std::uniform_real_distribution<float>& odist,
		            std::uniform_real_distribution<float>& rdist) :
			r(rdist(rng)),
			droll(ddist(rng)),
			dpitch(ddist(rng)),
			dyaw(ddist(rng)),
			dphi(odist(rng)),
			dtheta(odist(rng)),
			dchi(odist(rng)),
			chi(adist(rng)),
			theta(adist(rng)),
			phi(adist(rng)) { }




		mat4f getTransformMatrix() const noexcept override
		{
			return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			       dx::XMMatrixTranslation(r, 0.0f, 0.0f) * dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
		}


		void update(float delta) noexcept override
		{
			roll += droll * delta;
			pitch += dpitch * delta;
			yaw += dyaw * delta;
			theta += dtheta * delta;
			phi += dphi * delta;
			chi += dchi * delta;
		}

	private:
		// positional
		float r;
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float theta;
		float phi;
		float chi;
		// speed (delta/s)
		float droll;
		float dpitch;
		float dyaw;
		float dtheta;
		float dphi;
		float dchi;
	};
}
