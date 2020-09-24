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
 * File Name: common.h
 * Date File Created: 9/10/2020 at 3:38 PM
 * Author: Matt
 */

#ifndef MAGE3DX_COMMON_H
#define MAGE3DX_COMMON_H

//#define BMD_PROFILE 1

#include <bmd/types.h>


#ifndef MAGE_DEBUG
    #define MAGE_DEBUG 1
#endif

#ifndef MAGE_VERBOSE
    #define MAGE_VERBOSE 0
#endif


#define VOIDPP(x) reinterpret_cast<void**>(x)


#endif //MAGE3DX_COMMON_H