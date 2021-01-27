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
 * File Name: phongSpecPS.hlsl
 * Date File Created: 1/27/2021 at 11:31 AM
 * Author: Matt
 */

#include "lighting.hlsl"
#include "pointlight.hlsl"

cbuffer ModelCBuf
{
	float specularPower;
	bool hasGloss;
	float specularMapWeight;
}

Texture2D tex;
Texture2D spec;

SamplerState smpl;

float4 main(float3 viewPos : Position, float3 viewNormal : Normal,
	float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : TexCoords) : SV_Target
{
	viewNormal = normalize(viewNormal);
	// fragment to light vector data
	const LightVectorData lvd = calculate_light_vector(viewLightPos, viewPos);

	// Attenuation
	const float att = attenuate(attConst, attLin, attQuad, lvd.distToL);

	// diffuse intensity
	const float3 diff = diffuse(diffuseColor, diffuseIntensity, att, lvd.dirToL, viewNormal);

	float specPower = specularPower;
	const float4 specSample = spec.Sample(smpl, tc);
	const float3 specColorIntensity = specSample.rgb * specularMapWeight;
	if (hasGloss)
	{
		specPower = pow(2.0f, specSample.a * specPowerFactor);
	}
	const float3 specular = speculate(specColorIntensity, 1.0f, viewNormal, lvd.vToL, viewPos, att, specPower);

	return float4(saturate((diff + ambient) * tex.Sample(smpl, tc).rgb + specular * specColorIntensity), 1.0f);
}