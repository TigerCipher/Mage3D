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
 * File Name: TextureData.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "Bindable.h"


class TextureData : public Bindable
{
public:
	TextureData(Graphics& gfx, const std::string& path, uint slot = 0);
	void bind(Graphics &gfx) noexcept override;

	static SharedPtr<TextureData> resolve(Graphics& gfx, const std::string& path, uint slot = 0);
	static std::string generateUID(const std::string& path, uint slot = 0);
	std::string getUID() const noexcept override;

	[[nodiscard]] bool hasAlpha() const noexcept { return mAlpha; }

protected:
	COMptr<ID3D11ShaderResourceView> mTextureView;
	std::string mPath;
private:
	uint mSlot;
	bool mAlpha = false;
};



