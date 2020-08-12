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

#include <stdio.h>
#include <assert.h>
#include <errno.h>

#ifndef BMD_DEBUGGING
	#define BMD_DEBUGGING 0
#endif // BMD_DEBUGGING


#ifndef BMD_VERBOSE
	#define BMD_VERBOSE 0
#endif // BMD_VERBOSE


#if BMD_DEBUGGING || BMD_VERBOSE
#define BMD_ASSERT assert
#else
// If BMD_DEBUGGING is disabled (not 1) then don't use assert
#define BMD_ASSERT(...)
#endif // BMD_DEBUGGING || BMD_VERBOSE


#if BMD_DEBUGGING
	#define dbgprintln(fmt_, ...) printf(fmt_"\n", ##__VA_ARGS__)
	#define dbgprint(fmt_, ...) printf(fmt_, ##__VA_ARGS__)
	#define dbgprinterr(fmt_, ...) fprintf(stderr, fmt_, ##__VA_ARGS__)
#else
	#define dbgprintln(...)
	#define dbgprint(...)
	#define dbgprinterr(...)
#endif

#ifdef __cplusplus
	#define VOID_TO_CHAR (char*)
	#define VOID_TO_INT (int*)
#else
	#define VOID_TO_CHAR
	#define VOID_TO_INT
#endif

#endif //BMD_COMMON_H
