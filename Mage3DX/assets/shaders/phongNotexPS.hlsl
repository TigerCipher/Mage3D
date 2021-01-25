/*
 * Mage3DX
 * Copyright (C) 2021 Blue Moon Development. All rights reserved.
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
 * File Name: phongNotexPS.hlsl
 * Date File Created: 1/25/2021 at 1:03 PM
 * Author: Matt
 */

cbuffer LightCBuf
{
	float3 lightPos;
	float3 ambient;

	float3 diffuseColor;
	float diffuseIntensity;

	// Attenuation
	float attConst;
	float attLin;
	float attQuad;
}

cbuffer ModelCBuf
{
	float materialColor;
	float specularIntensity;
	float specularPower;
	float padding[2];
}


float4 main(float3 viewPos : Position, float3 n : Normal) : SV_Target
{
	// fragment to light vector data
	const float3 vToL = lightPos - viewPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;

	// Attenuation
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));

	// diffuse intensity
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));

	// Light reflection vectors
	const float3 w = n * dot(vToL, n);
	const float3 r = w * 2.0f - vToL;

	// Specular intensity
	const float3 spec = att * (diffuseColor * diffuseIntensity) * specularIntensity
		* pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);

	return float4(saturate((diffuse + ambient) * materialColor + spec), 1.0f);
	
}