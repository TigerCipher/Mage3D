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
 * File Name: lighting.hlsl
 * Date File Created: 1/26/2021 at 8:20 PM
 * Author: Matt
 */

static const float specPowerFactor = 13.0f;

struct LightVectorData
{
	float3 vToL;
	float3 dirToL;
	float distToL;
};


LightVectorData calculate_light_vector(const in float3 lightPos, const in float3 fragPos)
{
	LightVectorData lvd;
	lvd.vToL = lightPos - fragPos;
	lvd.distToL = length(lvd.vToL);
	lvd.dirToL = lvd.vToL / lvd.distToL;

	return lvd;
}


float3 normal_map_viewspace(const in float3 viewTan,
	const in float3 viewBitan,
	const in float3 viewNormal,
	const in float2 tc, uniform Texture2D norm, uniform SamplerState smpl)
{
	const float3x3 tanToView = float3x3(viewTan, viewBitan, viewNormal);
	const float3 normalSample = norm.Sample(smpl, tc).rgb;

	const float3 tanNormal = normalSample * 2.0f - 1.0f;

	return normalize(mul(tanNormal, tanToView));
}

float attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
	return 1.0f / (attConst + attLin * distFragToL + attQuad * (distFragToL * distFragToL));
}

float3 diffuse(uniform float3 diffuseColor, uniform float diffuseIntensity,
	const in float att, const in float3 viewDirToL, const in float3 viewNormal)
{
	return diffuseColor * diffuseIntensity * att * max(0.0f, dot(viewDirToL, viewNormal));
}

float3 speculate(const in float3 specularColor, uniform float specularIntensity,
	const in float3 viewNormal, const in float3 viewToL, const in float3 viewPos,
	const in float att, const in float specularPower)
{
	const float3 w = viewNormal * dot(viewToL, viewNormal);
	const float3 r = normalize(w * 2.0f - viewToL);
	const float3 viewCamToFrag = normalize(viewPos);

	return att * specularColor * specularIntensity * pow(max(0.0f, dot(-r, viewCamToFrag)), specularPower);
}