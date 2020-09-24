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
 * File Name: bindable.h
 * Date File Created: 9/20/2020 at 9:14 PM
 * Author: Matt
 */

#ifndef MAGE3DX_BINDABLE_H
#define MAGE3DX_BINDABLE_H


//#include "pch.h"
#include "mage/graphics/graphics.h"


namespace mage
{
    class Bindable
    {
    public:
        virtual ~Bindable() = default;
        virtual void bind(Graphics& gfx) noexcept = 0;

    protected:
        static ID3D11DeviceContext* getContext(Graphics& gfx) noexcept;
        static ID3D11Device* getDevice(Graphics& gfx) noexcept;
        static DebugInfo& getDebugInfo(Graphics& gfx) noexcept(MAGE_DEBUG);
    };

}// namespace mage


#endif//MAGE3DX_BINDABLE_H
