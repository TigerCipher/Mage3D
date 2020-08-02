/*
 * BMD
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
 * Date File Created: 7/30/2020 at 4:45 PM
 * Author: Matt
 */

#ifndef BMD_COMMON_H
#define BMD_COMMON_H


#ifndef DEBUGGING
	#define DEBUGGING 1
#endif // DEBUGGING


#ifndef BMD_VERBOSE
	#define BMD_VERBOSE 0
#endif // BMD_VERBOSE

// If DEBUGGING is 1
#if DEBUGGING || BMD_VERBOSE

#include <stdio.h>
#include <assert.h>
#include <errno.h>

#define BMD_ASSERT assert
#else
// If DEBUGGING is disabled (not 1) then don't use assert
#define BMD_ASSERT(...)
#endif // DEBUGGING

#endif //BMD_COMMON_H
