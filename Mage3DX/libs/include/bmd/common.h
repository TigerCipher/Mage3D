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

#ifndef BMD_ASSERTIONS
    #define BMD_ASSERTIONS 0
#endif // BMD_ASSERTIONS


#if BMD_ASSERTIONS
    #define BMD_ASSERT assert
#else
    #define BMD_ASSERT(...)
#endif

#define EXPAND_MACRO(x) x

#if BMD_DEBUGGING
    #ifndef BMD_WINDOWS
        #define dbgprintln(fmt_, ...) printf(fmt_"\n", __VA_ARGS__)
        #define dbgprint(fmt_, ...) printf(fmt_, __VA_ARGS__)
        #define dbgprinterr(fmt_, ...) fprintf(stderr, fmt_"\n", __VA_ARGS__)
    #else
        #define dbgprint_win(str) OutputDebugString(str)
        #define dbgprintln(...)
        #define dbgprint(...)
        #define dbgprinterr(...)
    #endif


    #define INTERNAL_ASSERT_IMPL(x, assert_msg, ...) if(!(x)) { dbgprinterr(assert_msg, __VA_ARGS__); }
    #define INTERNAL_ASSERT_WITH_MSG(x, ...) INTERNAL_ASSERT_IMPL(x, "Assertion failed: " __VA_ARGS__)
    #define INTERNAL_ASSERT_NO_MSG(x) INTERNAL_ASSERT_IMPL(x, "Assertion failed at %s:%i", __FILE__, __LINE__)

    #if !BMD_ASSERTIONS
        #define VFUNC2(x, ...) if(NUM_ARGS_(__VA_ARGS__) > 1){ INTERNAL_ASSERT_WITH_MSG(x, __VA_ARGS__)} \
        else { INTERNAL_ASSERT_NO_MSG(x) }
    #else
        #define VFUNC2(x, ...) if(NUM_ARGS_(__VA_ARGS__) > 1){ INTERNAL_ASSERT_WITH_MSG(x, __VA_ARGS__)} \
        else { INTERNAL_ASSERT_NO_MSG(x) } EXPAND_MACRO(BMD_ASSERT(x))
    #endif

    #define CHECK(x, ...) VFUNC2(x, __VA_ARGS__)

#else
    #define dbgprintln(...)
    #define dbgprint(...)
    #define dbgprinterr(...)
    #define CHECK(...)
#ifdef BMD_WINDOWS
    #define dbgprint_win(str)
#endif
#endif

#ifdef __cplusplus
    #define VOID_TO_CHAR (char*)
    #define VOID_TO_INT (int*)
#else
    #define VOID_TO_CHAR
    #define VOID_TO_INT
#endif


#define _GET_NTH_ARG(                                   \
      _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,          \
     _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,  \
     _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,  \
     _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,  \
     _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
     _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,  \
     _61, _62, _63, N, ...) N
#define REVERSE_ARGS_RANGE()        \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0
#define _NUM_ARGS(...) _GET_NTH_ARG(__VA_ARGS__)
#define NUM_ARGS(...) _NUM_ARGS(__VA_ARGS__,REVERSE_ARGS_RANGE())


#define STR(x) #x
#define STR_EXPANDED(x) STR(x)



#endif //BMD_COMMON_H
