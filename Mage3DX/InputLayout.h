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
 * File Name: InputLayout.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once


#include "Bindable.h"
#include "Vertex.h"

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, vtx::VertexLayout layout, ID3DBlob* vertexBytecode);
	void bind(Graphics& gfx) noexcept override;

	static SharedPtr<InputLayout> resolve(Graphics& gfx, const vtx::VertexLayout& layout, ID3DBlob* vsBytecode);
	static std::string generateUID(const vtx::VertexLayout& layout, ID3DBlob* vsBytecode = nullptr);

	std::string getUID() const noexcept override;

protected:
	COMptr<ID3D11InputLayout> mLayout;
	vtx::VertexLayout mVertexLayout;
};

