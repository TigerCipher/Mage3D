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
	float3 viewLightPos;
    float3 ambient;
	
	float3 diffuseColor;
    float diffuseIntensity;

	// Attenuation
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ModelCBuf
{
    float specularIntensity;
    float specularPower;
	float padding[2];
};

Texture2D tex;

SamplerState smpl;

float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float2 tc : TexCoords) : SV_Target
{
	viewNormal = normalize(viewNormal);
	
	// fragment to light vector data
    const float3 vToL = viewLightPos - viewPos;
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
    const float3 spec = att * (diffuseColor * diffuseIntensity) * specularIntensity 
		* pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);

	return float4(saturate((diffuse + ambient) * tex.Sample(smpl, tc).rgb + spec), 1.0f);
}