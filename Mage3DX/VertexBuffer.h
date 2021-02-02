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
 * File Name: VertexBuffer.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once


#include "GraphicsException.h"
#include "Bindable.h"
#include "Vertex.h"


class VertexBuffer : public Bindable
{
public:
	VertexBuffer(Graphics& gfx, const std::string& tag, const vtx::VertexBuffer& vData);
	VertexBuffer(Graphics& gfx, const vtx::VertexBuffer& vData);

	void bind(Graphics& gfx) noexcept override;

	static SharedPtr<VertexBuffer> resolve(Graphics& gfx, const std::string& tag, const vtx::VertexBuffer& vData);

	template<typename ... Ignore>
	static std::string generateUID(const std::string& tag, Ignore&&... ignore)
	{
		return generateUID_(tag);
	}

	std::string getUID() const noexcept override;

protected:
	std::string mTag;
	UINT mStride;
	COMptr<ID3D11Buffer> mBuffer{ };

private:
	static std::string generateUID_(const std::string& tag);
};
