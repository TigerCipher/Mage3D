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
 * File Name: mathhelper.h
 * Date File Created: 9/20/2020 at 2:47 PM
 * Author: Matt
 */

#ifndef MAGE3DX_MATHHELPER_H
#define MAGE3DX_MATHHELPER_H


//#include "pch.h"

#include <DirectXMath.h>
#include <cmath>

namespace dx = DirectX;

typedef dx::XMMATRIX mat4f;
typedef dx::XMVECTOR vec4f;
typedef dx::XMFLOAT3 vec3f;
typedef dx::XMFLOAT2 vec2f;

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;


template<typename T>
constexpr auto pow2(const T& x)
{
    return x * x;
}

template<typename T>
constexpr T interpolate(const T& src, const T& dest, float alpha)
{
    return src + (dest - src) * alpha;
}

template<typename T>
constexpr T toRadians(T degrees)
{
    return degrees * PI / (T) 180.0;
}

template<typename T>
T wrapAngle(T theta)
{
    const T modded = fmod(theta, (T) 2.0 * (T) PI_D);
    return (modded > (T) PI_D) ? (modded - (T) 2.0 * (T)PI_D) : modded;
}




namespace mage
{
    float dot(vec4f v1, vec4f v2);
}

#ifdef MATH_HELPER_IMPL
#ifndef MATH_HELPER_IMPL_ONCE
    #define MATH_HELPER_IMPL_ONCE
namespace mage
{
    float dot(vec4f v1, vec4f v2)
    {
        auto result = dx::XMVector4Dot(v1, v2);
        auto d = dx::XMVectorGetX(result);
        return d;
    }
}
#endif
#endif


#endif //MAGE3DX_MATHHELPER_H
