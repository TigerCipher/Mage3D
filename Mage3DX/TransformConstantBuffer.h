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
 * File Name: TransformConstantBuffer.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once


#include "ConstantBuffer.h"
#include "IRenderable.h"
#include "MathHelper.h"

class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& gfx, const IRenderable& parent, UINT slot = 0);
	void bind(Graphics& gfx) noexcept override;

protected:
	struct Transforms
	{
		mat4f model;
		mat4f mvp;
	};

	Transforms getTransforms(Graphics& gfx) noexcept;

private:
	static UniquePtr<VertexConstantBuffer<Transforms> > sVertexBuffer;
	const IRenderable& mParent;
};


class TransformPixelConstantBuffer : public TransformConstantBuffer
{
public:
	TransformPixelConstantBuffer(Graphics& gfx, const IRenderable& parent, UINT slotv = 0, UINT slotp = 0);
	void bind(Graphics& gfx) noexcept override;

private:
	static UniquePtr<PixelConstantBuffer<Transforms>> sPixelBuffer;
};

