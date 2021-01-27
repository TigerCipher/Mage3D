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
 * File Name: phongNormalObjPS.hlsl
 * Date File Created: 1/24/2021 at 12:35 AM
 * Author: Matt
 */

#include "lighting.hlsl"
#include "pointlight.hlsl"


cbuffer ModelCBuf
{
	float specularIntensity;
	float specularPower;
	bool normalMapEnabled;
	float padding[1];
}

#include "transform.hlsl"

Texture2D tex;
Texture2D norm : register(t2);

SamplerState smpl;

float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float2 tc : TexCoords) : SV_Target
{


	if (normalMapEnabled)
	{
		const float3 normalSample = norm.Sample(smpl, tc).xyz;
		const float3 objNormal = normalSample * 2.0f - 1.0f;

		viewNormal = normalize(mul(objNormal, (float3x3) modelView));
	}

	const LightVectorData lvd = calculate_light_vector(viewLightPos, viewPos);

	// Attenuation
	const float att = attenuate(attConst, attLin, attQuad, lvd.distToL);

	const float3 diff = diffuse(diffuseColor, diffuseIntensity, att, lvd.dirToL, viewNormal);

	// Specular intensity
	const float3 spec = speculate(specularIntensity.rrr, 1.0f, viewNormal, lvd.vToL, viewPos, att, specularPower);

	return float4(saturate((diff + ambient) * tex.Sample(smpl, tc).rgb + spec), 1.0f);
}