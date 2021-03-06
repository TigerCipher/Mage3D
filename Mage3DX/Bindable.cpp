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
 * File Name: bindable.cpp
 * Date File Created: 9/20/2020 at 9:14 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "Bindable.h"


ID3D11DeviceContext* Bindable::getContext(Graphics& gfx) noexcept
{
    return gfx.mContext.Get();
}

ID3D11Device* Bindable::getDevice(Graphics& gfx) noexcept
{
    return gfx.mDevice.Get();
}

DebugInfo& Bindable::getDebugInfo(Graphics& gfx) noexcept(MAGE_DEBUG)
{
    #if MAGE_DEBUG
    return gfx.mDebugInfo;
    #else
    throw std::logic_error("The Debug Info system is only available in debug builds. You are using a release build");
    #endif

}
