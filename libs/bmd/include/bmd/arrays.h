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
 * File Name: arrays.h
 * Date File Created: 7/31/2020 at 5:53 PM
 * Author: Matt
 */

#ifndef BMD_ARRAYS_H
#define BMD_ARRAYS_H

/*
* In no way should these functions be considered 100% fool proof or concrete.
* In fact, I'm 87.9467% sure the following defines are OS specific, maybe compiler specific, won't know
* until I have more time to test.
*
* In addition, none of these match the minimum value allowed for the given type. If the array contains
* the defined null terminating value, it will stop counting and the returned length will come up short
* With the exception of NULL_TERMINATING_CHAR as \0 marks the end of a char array regardless of OS/compiler
*/

#include "bmd/types.h"


/**
* The callback function pointer of type T that gets called by getLength to determine if the value
* has reached a null terminating point
*/
template<typename T>
using length_callback = int (const T* arr);



/**
* A proper char array is ended by the null character \0 (int 0).
* A case could be that someone has done:
* char* dynArray = new char[5];
* then filled it out but never filled in the \0,
* In this case the function will ultimately fail as the char at dynArray[5] will be -3 (superscript 2)
* While uncommon, its more common than the other null terminating values I tested
*/
#define NULL_TERMINATING_CHAR 0

/**
* From my testing, this value comes immediately after the last actual value in a short array when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_SHORT -13108

/**
* From my testing, this value comes immediately after the last actual value in a short array that was
* dynamically allocated (new short[#]) when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_SHORT_NEW -515

/**
* From my testing, this value comes immediately after the last actual value in an integer array when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_INT -858993460

/**
* From my testing, this value comes immediately after the last actual value in an integer array that was
* dynamically allocated (new int[#]) when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_INT_NEW -33686019

// Same value as NULL_TERMINATING_INT due to same reasoning listed below for double and long double

/**
* From my testing, this value comes immediately after the last actual value in a long array when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_LONG -858993460

/**
* From my testing, this value comes immediately after the last actual value in a long array that was
* dynamically allocated (new long[#]) when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_LONG_NEW -33686019


/**
* From my testing, this value comes immediately after the last actual value in a long long array when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_LONG_LONG -3689348814741910324

/**
* From my testing, this value comes immediately after the last actual value in a long long array that was
* dynamically allocated (new long long[#]) when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_LONG_LONG_NEW 2284888915453

/**
* From my testing, this value comes immediately after the last actual value in a float array when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_FLOAT -107374176.0f

/**
* From my testing, this value comes immediately after the last actual value in a float array that was
* dynamically allocated (new float[#]) when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_FLOAT_NEW -42201683186052843888412332559599403008.0f


/**
* From my testing, this value comes immediately after the last actual value in a double array when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_DOUBLE -92559631349317830736831783200707727132248687965119994463780864.0
// On 64 bit Visual C++ compiler, long double and double are both 8 bytes, I believe that is why they share this value
// Perhaps the case could be the byte allocation required for the type? In which case, could make this independent
// with some #ifdefs for the compiler type?

/**
* From my testing, this value comes immediately after the last actual value in a long double array when it is
* decayed to a pointer due to being passed as a parameter
*/
#define NULL_TERMINATING_LONG_DOUBLE -92559631349317830736831783200707727132248687965119994463780864.0

// Null terminating for double and long double when using 'new' (double* d = new double[#]) is 0.0
// so doesn't look like we can support that
// Though at some point it reaches '-nan' maybe that can be detected and return -1 as the size?

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @tparam T The type of data stored in the array
* @param arr The array
* @param lcb The length_callback function pointer to be called when checking for the null terminating value
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
template<typename T>
extern int getLength(const T* arr, length_callback<T> lcb);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const char* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const short* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const int* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const long* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const long long* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const uchar* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const ushort* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const uint* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const ulong* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const ulonglong* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const flt32* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const flt64* arr);

/**
* Attempts to retrieve the length of an array, even if its been decayed to a pointer by being passed as a function parameter.
*
* It should be noted that this is not 100% foolproof and only works if the defined null terminating values are not values in the array.
* In addition, compilers allocate memory in different ways, so the default null terminating values may not be correct for your compiler
* @param arr The array
* @return The possible length of the array, or -1 if it could not be estimated or if the array was null
*/
extern int getLength(const flt96* arr);


#endif //BMD_ARRAYS_H
