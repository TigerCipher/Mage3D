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
 * File Name: Blender.h
 * Date File Created: 1/29/2021 at 8:20 PM
 * Author: Matt
 */

#pragma once

#include "Bindable.h"

class Blender : public Bindable
{
public:
	Blender(Graphics& gfx, bool blending, std::optional<float> factor = {});
	void bind(Graphics& gfx) noexcept override;

	void setFactor(float factor) NOX;

	float getFactor() const NOX;
	
	static SharedPtr<Blender> resolve(Graphics& gfx, bool blending, std::optional<float> factor = {});
	static std::string generateUID(bool blending, std::optional<float> factor = {});
	std::string getUID() const noexcept override;
protected:
	COMptr<ID3D11BlendState> mBlender;
	bool mBlending;
	std::optional<std::array<float, 4>> mFactors;
};
