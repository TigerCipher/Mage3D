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
	float specularIntensity;
	float specularPower;
	bool normalMapEnabled;
	float padding[1];
}

cbuffer TransformCBuf
{
	matrix model;
	matrix mvp;
}

Texture2D tex;
Texture2D norm : register(t2);

SamplerState smpl;

float4 main(float3 viewPos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : TexCoords) : SV_Target
{


	if (normalMapEnabled)
	{
		const float3x3 tanToView = float3x3(normalize(tan), normalize(bitan), normalize(n));
		const float3 normalSample = norm.Sample(smpl, tc);
		n.x = normalSample.x * 2.0f - 1.0f;
		n.y = -normalSample.y * 2.0f + 1.0f;
		n.z = normalSample.z * 2.0f - 1.0f;

		n = mul(n, tanToView);
	}
	
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

	return float4(saturate((diffuse + ambient) * tex.Sample(smpl, tc).rgb + spec), 1.0f);
}