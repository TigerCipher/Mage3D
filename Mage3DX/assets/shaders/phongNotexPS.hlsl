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
	float4 materialColor;
	float4 specularColor;
	float specularPower;
}


float4 main(float3 viewPos : Position, float3 viewNormal : Normal) : SV_Target
{

	viewNormal = normalize(viewNormal);
	
	// fragment to light vector data
	const float3 vToL = lightPos - viewPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;

	// Attenuation
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));

	// diffuse intensity
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, viewNormal));

	// Light reflection vectors
	const float3 w = viewNormal * dot(vToL, viewNormal);
	const float3 r = w * 2.0f - vToL;

	// Specular intensity
	const float4 spec = att * (float4(diffuseColor, 1.0f) * diffuseIntensity) * specularColor
		* pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);

	return saturate(float4(diffuse + ambient, 1.0f) * materialColor + spec);
	
}