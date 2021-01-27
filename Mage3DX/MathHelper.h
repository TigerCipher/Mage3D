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
 * File Name: MathHelper.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once


#include <DirectXMath.h>
#include <cmath>

namespace dx = DirectX;

typedef dx::XMMATRIX mat4f;
typedef dx::XMFLOAT4X4 mat4x4;
typedef dx::XMFLOAT3X3 mat3x3;
typedef dx::XMFLOAT4X3 mat4x3;
typedef dx::XMFLOAT3X4 mat3x4;


typedef dx::XMVECTOR vec;
typedef dx::XMFLOAT4 vec4f;
typedef dx::XMFLOAT3 vec3f;
typedef dx::XMFLOAT2 vec2f;

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;


// The following constexprs/macros have no real purpose other than personal convenience
// and potential future compatibility between both OpenGL and DirectX math libraries
// Stuff will be added here as I find the need/desire for it

const auto storeVector = dx::XMStoreFloat;
const auto storeVector4 = dx::XMStoreFloat4;
const auto storeVector3 = dx::XMStoreFloat3;
const auto storeVector2 = dx::XMStoreFloat2;
const auto loadVector = dx::XMLoadFloat;
const auto loadVector4 = dx::XMLoadFloat4;
const auto loadVector3 = dx::XMLoadFloat3;
const auto loadVector2 = dx::XMLoadFloat2;

const auto rollPitchYaw = dx::XMMatrixRotationRollPitchYaw;
const auto translateMatrix = dx::XMMatrixTranslation;
const auto scaleMatrix = dx::XMMatrixScaling;
const auto transposeMatrix = dx::XMMatrixTranspose;
const auto rotateXMatrix = dx::XMMatrixRotationX;
const auto rotateYMatrix = dx::XMMatrixRotationY;
const auto rotateZMatrix = dx::XMMatrixRotationZ;

const auto normalizeVector3 = dx::XMVector3Normalize;
const auto crossVector3 = dx::XMVector3Cross;
const auto dotVector3 = dx::XMVector3Dot;

const auto subVector = dx::XMVectorSubtract;
const auto mulVector = dx::XMVectorMultiply;
const auto divVector = dx::XMVectorDivide;
const auto addVector = dx::XMVectorAdd;

const auto transformVector3 = dx::XMVector3Transform;

const auto setVector = dx::XMVectorSet;

const auto replicateVector = dx::XMVectorReplicate;

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
	return degrees * PI / static_cast<T>(180.0);
}

template<typename T>
T wrapAngle(T theta)
{
	constexpr T dblPi = static_cast<T>(2) * static_cast<T>(PI);
	const T mod = fmod(theta, dblPi);
	if (mod > static_cast<T>(PI_D))
		return mod - dblPi;
	if (mod < static_cast<T>(PI_D))
		return mod + dblPi;
	return mod;
}


extern float dot(vec v1, vec v2);

extern vec3f extract_euler_angles(const mat4x4& matrix);
extern vec3f extract_translation(const mat4x4& matrix);