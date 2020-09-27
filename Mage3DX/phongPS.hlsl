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
 * File Name: phongPS.hlsl
 * Date File Created: 9/26/2020 at 4:02 PM
 * Author: Matt
 */
cbuffer LightCBuf
{
	float3 lightPos;
};

static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;

// Attenuation
static const float attConst = 1.0f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
	// fragment to light vector data
	const float3 vToL = lightPos - worldPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;

	// diffuse attenuation
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));

	// diffuse intensity
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));

	return float4(saturate(diffuse + ambient), 1.0f);
}