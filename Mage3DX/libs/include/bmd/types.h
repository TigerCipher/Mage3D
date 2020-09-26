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
 * File Name: types.h
 * Date File Created: 7/29/2020 at 10:29 PM
 * Author: Matt
 */


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

#ifndef BMD_TYPES_H
#define BMD_TYPES_H

#include <stdint.h>

// A lot of stuff is currently only functional for C++ so might as well add some of my common
// C++ typedefs and whatnot until I have time to make everything compatible for C


#define MIN_SBYTE -128
#define MAX_SBYTE 127
#define MIN_UBYTE 0
#define MAX_UBYTE 255

// Rename the fixed width integer types
// To me, _f makes more sense than _t since its fixed. _t makes it seem like it might not be fixed
typedef int8_t int8_f; // signed char
typedef int16_t int16_f; // signed short
typedef int32_t int32_f; // signed int
typedef int64_t int64_f; // signed long long
typedef uint8_t uint8_f; // unsigned char
typedef uint16_t uint16_f; // unsigned short
typedef uint32_t uint32_f; // unsigned int
typedef uint64_t uint64_f; // unsigned long long

typedef unsigned char ubyte;
typedef signed char sbyte;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;


// If my understanding is correct, these aren't necessarily fixed like uint#_t
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

// In case someone wants to refer to byte size rather than bit size
typedef unsigned char uint_1;
typedef unsigned short uint_2;
typedef unsigned int uint_4;
typedef unsigned long long uint_8;
typedef char int_1;
typedef short int_2;
typedef int int_4;
typedef long long int_8;


// Floating point types
typedef float flt32;
typedef double flt64;
typedef long double flt96;




// Because I use "Lists" in Java more than I use vectors in C++,
// this is just simply a small rewrite of the vector for quality of life sake
#ifdef __cplusplus

#include <vector>
#include <memory>

template<typename T>
using list = std::vector<T>;

template <typename T>
using UniquePtr = std::unique_ptr<T>;
template <typename T>
using uptr = std::unique_ptr<T>;
// If my understanding is correct, a unique pointer only exists in a single scope and not referenced else where
// So the following are naming conventions just to help myself remember that, as I've only recently begun using smart pointers in C++
// instead of raw pointers
template <typename T>
using scope = std::unique_ptr<T>;
template <typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;
template<typename T>
using sptr = std::shared_ptr<T>;
// If my understanding is correct, a shared pointer can be "owned" by multiple sources and has a reference counter
// So the following are naming conventions just to help myself remember that, as I've only recently begun using smart pointers in C++
// instead of raw pointers
template<typename T>
using ref = std::shared_ptr<T>;
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr UniquePtr<T> createScope(Args&& ...args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
constexpr SharedPtr<T> createRef(Args&& ...args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}


#endif // __cplusplus

#endif //BMD_TYPES_H
