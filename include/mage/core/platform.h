/*
 * Mage3D
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
 * File Name: platform.h
 * Date File Created: 7/20/2020 at 10:52 PM
 * Author: Matt
 */

#ifndef MAGE3D_PLATFORM_H
#define MAGE3D_PLATFORM_H

#include <stdint.h>

// I hate having to add _t. What does the _t even stand for?
// Rename the fixed width integer types
typedef int8_t int8; // signed char
typedef int16_t int16; // signed short
typedef int32_t int32; // signed int
typedef int64_t int64; // signed long long
typedef uint8_t uint8; // unsigned char
typedef uint16_t uint16; // unsigned short
typedef uint32_t uint32; // unsigned int
typedef uint64_t uint64; // unsigned long long

typedef signed char sByte;
typedef unsigned char uByte;

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#define MIN_SBYTE -127
#define MAX_SBYTE 127
#define MIN_UBYTE 0
#define MAX_UBYTE 255

// Max/min values got with:
/* (from stdint.h)
 * #define INT8_MIN         (-127i8 - 1)
#define INT16_MIN        (-32767i16 - 1)
#define INT32_MIN        (-2147483647i32 - 1)
#define INT64_MIN        (-9223372036854775807i64 - 1)
#define INT8_MAX         127i8
#define INT16_MAX        32767i16
#define INT32_MAX        2147483647i32
#define INT64_MAX        9223372036854775807i64
#define UINT8_MAX        0xffui8
#define UINT16_MAX       0xffffui16
#define UINT32_MAX       0xffffffffui32
#define UINT64_MAX       0xffffffffffffffffui64*/

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS) || defined(__WIN32__) || defined(__WINDOWS__) || defined(__TOS_WIN__)
#define OS_NAME "Windows"
#define OS_WINDOWS

#elif defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__)
#define OS_NAME "Apple"
#define OS_APPLE

#elif defined(__unix__) || defined(__unix) || defined(__linux__) || defined(linux) || defined(__linux)
#define OS_NAME "Unix"
#define OS_UNIX
#else
#define OS_NAME "Other"
#define OS_OTHER
#endif // OS Check



#endif //MAGE3D_PLATFORM_H
