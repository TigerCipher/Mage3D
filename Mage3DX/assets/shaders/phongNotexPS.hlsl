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

#include "lighting.hlsl"
#include "pointlight.hlsl"

cbuffer ModelCBuf
{
	float4 materialColor;
	float4 specularColor;
	float specularPower;
}


float4 main(float3 viewPos : Position, float3 viewNormal : Normal) : SV_Target
{

	viewNormal = normalize(viewNormal);
	
	const LightVectorData lvd = calculate_light_vector(viewLightPos, viewPos);

	const float att = attenuate(attConst, attLin, attQuad, lvd.distToL);

	const float3 diff = diffuse(diffuseColor, diffuseIntensity, att, lvd.dirToL, viewNormal);

	const float3 spec = speculate(diffuseColor, diffuseIntensity, viewNormal, lvd.vToL, viewPos, att, specularPower);

	return float4(saturate(float4(diff + ambient, 1.0f) * materialColor + spec), 1.0f);
	
}