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
 * File Name: math_helper.cpp
 * Date File Created: 9/25/2020 at 5:55 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "MathHelper.h"

float dot(vec v1, vec v2)
{
    auto result = dx::XMVector4Dot(v1, v2);
    auto d = dx::XMVectorGetX(result);
    return d;
}