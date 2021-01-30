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
 * File Name: phongSpecNormalPS.hlsl
 * Date File Created: 1/21/2021 at 8:48 PM
 * Author: Matt
 */

#include "pointlight.hlsl"
#include "lighting.hlsl"

cbuffer ModelCbuf
{
	bool normalMapEnabled;
	bool specularMapEnabled;
	bool hasGloss;
	float specularPower;
	float3 specularColor;
	float specularMapWeight;
}

Texture2D tex;
Texture2D spec;
Texture2D norm;

SamplerState smpl;


float4 main(float3 viewPos : Position, float3 viewNormal : Normal,
			float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : TexCoords) : SV_Target
{
	float4 diftex = tex.Sample(smpl, tc);
	clip(diftex.a < 0.1f ? -1 : 1);

	if(normalMapEnabled)
	{
		viewNormal = normal_map_viewspace(
			normalize(tan), 
			normalize(bitan), 
			normalize(viewNormal),
			tc, norm, smpl);
	}else
	{
		viewNormal = normalize(viewNormal);
	}

	
	// fragment to light vector data
	const LightVectorData lvd = calculate_light_vector(viewLightPos, viewPos);

	// Attenuation
	const float att = attenuate(attConst, attLin, attQuad, lvd.distToL);

	// diffuse intensity
	const float3 diff = diffuse(diffuseColor, diffuseIntensity, att, lvd.dirToL, viewNormal);

	// Specular intensity
	float3 specColorIntensity;

	float specPower = specularPower;
	if(specularMapEnabled)
	{
		const float4 specSample = spec.Sample(smpl, tc);
		specColorIntensity = specSample.rgb * specularMapWeight;
		if(hasGloss)
		{
			specPower = pow(2.0f, specSample.a * specPowerFactor);	
		}
	}else
	{
		specColorIntensity = specularColor;
	}
	const float3 specular = speculate(specColorIntensity, 1.0f, viewNormal, lvd.vToL, viewPos, att, specPower);

	
	return float4(saturate((diff + ambient) * diftex.rgb + specular * specColorIntensity), diftex.a);
}