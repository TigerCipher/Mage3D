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
 * File Name: phongNotexVS.hlsl
 * Date File Created: 1/25/2021 at 1:00 PM
 * Author: Matt
 */


cbuffer CBuf
{
	matrix modelView;
	matrix mvp;
}

struct VSOut
{
	float3 viewPos : Position;
	float3 normal : Normal;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal)
{
	VSOut vso;
	vso.viewPos = (float3) mul(float4(pos, 1), modelView);
	vso.normal = mul(n, (float3x3) modelView);
	vso.pos = mul(float4(pos, 1), mvp);

	return vso;
}
