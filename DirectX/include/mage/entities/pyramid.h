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
 * File Name: pyramid.h
 * Date File Created: 9/21/2020 at 11:44 PM
 * Author: Matt
 */

#ifndef MAGE3DX_PYRAMID_H
#define MAGE3DX_PYRAMID_H


#include "pch.h"
#include "mage/graphics/renderable.h"

namespace mage
{
    class Pyramid : public Renderable<Pyramid>
    {
    public:
        Pyramid(Graphics& gfx, std::mt19937& rng,
                std::uniform_real_distribution<float>& adist,
                std::uniform_real_distribution<float>& ddist,
                std::uniform_real_distribution<float>& odist,
                std::uniform_real_distribution<float>& rdist);

        mat4f getTransformMatrix() const noexcept override;
        void update(float delta) noexcept override;

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


#endif //MAGE3DX_PYRAMID_H
