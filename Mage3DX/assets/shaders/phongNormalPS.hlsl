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
 * File Name: phongNormalPS.hlsl
 * Date File Created: 1/23/2021 at 8:04 PM
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


Texture2D tex;
Texture2D norm : register(t2);

SamplerState smpl;

float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : TexCoords) : SV_Target
{


	if (normalMapEnabled)
	{
		viewNormal = normal_map_viewspace(normalize(tan), 
			normalize(bitan), 
			normalize(viewNormal),
			tc, norm, smpl);
	}
	
	const LightVectorData lvd = calculate_light_vector(viewLightPos, viewPos);

	const float att = attenuate(attConst, attLin, attQuad, lvd.distToL);

	const float3 diff = diffuse(diffuseColor, diffuseIntensity, att, lvd.dirToL, viewNormal);

	const float3 spec = speculate(diffuseColor, diffuseIntensity, viewNormal, lvd.vToL, viewPos, att, specularPower);

	return float4(saturate((diff + ambient) * tex.Sample(smpl, tc).rgb + spec), 1.0f);
}