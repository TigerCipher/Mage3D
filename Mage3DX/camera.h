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
 * File Name: camera.h
 * Date File Created: 9/25/2020 at 8:10 PM
 * Author: Matt
 */

#ifndef MAGE3DX_CAMERA_H
#define MAGE3DX_CAMERA_H

#include "graphics.h"
#include "math_helper.h"

namespace mage
{
    class Camera
    {
    public:
        Camera() = default;
        virtual ~Camera() = default;

        void spawnControlWindow() noexcept;
        void reset() noexcept;

        [[nodiscard]] mat4f getViewMatrix() const noexcept;


    private:
        float m_r = 20.0f;
        float m_theta = 0.0f;
        float m_phi = 0.0f;
        float m_pitch = 0.0f;
        float m_yaw = 0.0f;
        float m_roll = 0.0f;
    };

}


#endif //MAGE3DX_CAMERA_H
