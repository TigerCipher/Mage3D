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
 * File Name: IndexBuffer.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const list<ushort>& indices);
	IndexBuffer(Graphics& gfx, std::string tag, const list<ushort>& indices);
	void bind(Graphics& gfx) noexcept override;
	[[nodiscard]] inline UINT getCount() const noexcept { return mCount; }

	static SharedPtr<IndexBuffer> resolve(Graphics& gfx, const std::string& tag, const list<ushort>& indices);

	template<typename... Ignore>
	static std::string generateUID(const std::string& tag, Ignore&&...ignore)
	{
		return generateUID_(tag);
	}

	std::string getUID() const noexcept override;
protected:
	UINT mCount;
	COMptr<ID3D11Buffer> mBuffer;
	std::string mTag;

private:
	static std::string generateUID_(const std::string& tag);
};
