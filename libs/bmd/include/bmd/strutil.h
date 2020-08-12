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
 * File Name: strutil.h
 * Date File Created: 7/30/2020 at 12:37 AM
 * Author: Matt
 */

#ifndef BMD_STRUTIL_H
#define BMD_STRUTIL_H

#define NULL_TERM '\0'
#define NULL_TERM_SIZE sizeof(char)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
* Overwrites the data in the destination string with the data from the src string
* @param dest The string to overwrite. Must be a fixed string, not dynamic.
* @param src The source string to copy to the destination
* @param max The maximum allowed length to copy for use cases where this can effect safety
* @return The size of the new destination string or -13 if the src is longer than the given max
*/
int copyStr_s(char* dest, const char* src, int max);

/**
* Overwrites the data in the destination string with the data from the src string
* @param dest The string to overwrite. Must be a fixed string, not dynamic.
* @param src The source string to copy to the destination
* @return The size of the new destination string
*/
int copyStr(char* dest, const char* src);


/**
* Overwrites the data in the destination string with the data from the src string
* <p>
* This is safe only in the sense that it will not copy past the given max, but due to dynamic allocation being used,
* when your code is done with the copied string (dest), be sure to call free(dest)!
* </p>
* @param dest The string to overwrite
* @param src The source string to copy to the destination
* @param max The maximum allowed length to copy for use cases where this can effect safety
* @return The size of the new destination string or -13 if the src is longer than the given max
*/
int copyStrDynamic_s(char** dest, const char* src, int max);


/**
* Overwrites the data in the destination string with the data from the src string
* <p>
* Due to dynamic allocation being used, when your code is done with the copied string (dest),
* be sure to call free(dest)!
* </p>
* @param dest The string to overwrite
* @param src The source string to copy to the destination
* @return The size of the new destination string
*/
int copyStrDynamic(char** dest, const char* src);

/**
* Concatenates two C-Style strings
* @param orig The original C-String. Must not be a dynamic array
* @param add The C-String to append to orig
* @return 0 if no error, non-zero if error
*/
int concatStr(char* orig, const char* add);

/**
* Concatenates two C-Style strings
* @param orig The original C-String. Must not be a dynamic array
* @param add The C-String to append to orig
* @param start The index to start concatenating at
* @param stop The index to stop concatenating at
* @return 0 if no error, non-zero if error
*/
int concatStr_r(char* orig, const char* add, int start, int stop);

/**
* Concatenates two C-Style strings
* @param orig The original C-String. Must not be a dynamic array
* @param add The C-String to append to orig
* @param stop The index to stop concatenating at
* @return 0 if no error, non-zero if error
*/
int concatStr_to(char* orig, const char* add, int stop);

/**
* Concatenates two C-Style strings
* <p>
* Due to dynamic allocation being used, when your code is done with the copied string (dest),
* be sure to call free(dest)!
* </p>
* @param orig Pointer to the original C-String. Meant for dynamic arrays, if the array is static with a defined size use concatStr
* @param add The C-String to append to orig
* @return 0 if no error, non-zero if error
*/
int concatStrDynamic(char** orig, const char* add);

/**
* Concatenates two C-Style strings
* <p>
* Due to dynamic allocation being used, when your code is done with the copied string (dest),
* be sure to call free(dest)!
* </p>
* @param orig Pointer to the original C-String. Meant for dynamic arrays, if the array is static with a defined size use concatStr
* @param add The C-String to append to orig
* @param start The index to start concatenating at
* @param stop The index to stop concatenating at
* @return 0 if no error, non-zero if error
*/
int concatStrDynamic_r(char** orig, const char* add, int start, int stop);

/**
* Concatenates two C-Style strings
* <p>
* Due to dynamic allocation being used, when your code is done with the copied string (dest),
* be sure to call free(dest)!
* </p>
* @param orig Pointer to the original C-String. Meant for dynamic arrays, if the array is static with a defined size use concatStr
* @param add The C-String to append to orig
* @param stop The index to stop concatenating at
* @return 0 if no error, non-zero if error
*/
int concatStrDynamic_to(char** orig, const char* add, int stop);

/**
* Captures a sub string contained within str ranging from [start - (stop - 1)]
* @param str The string containing the desired sub string
* @param start The beginning index of the sub string, inclusive
* @param stop The ending index of the sub string, exclusive
* @return The substring, or NULL if either index were out of bounds
*/
char* substr(const char* str, int start, int stop);

/**
* Captures a sub string contained within str starting from the given start index
* @param str The string containing the desired sub string
* @param start The beginning index of the sub string, inclusive
* @return The substring, or NULL if either index were out of bounds
*/
char* substrFrom(const char* str, int start);

/**
* Retrieves the first index where the given character exists
* @param str The string containing the character
* @param c The character to look for
* @return The first index of the character, or an error code (less than 0) if its not found
*/
int indexOf(const char* str, char c);

/**
* Retrieves the last index where the given character exists
* @param str The string containing the character
* @param c The character to look for
* @return The last index of the character, or an error code (less than 0) if its not found
*/
int lastIndexOf(const char* str, char c);


/**
* Retrieves every index where the given character exists and stores them in an int* (array)
* @param str The string containing the character
* @param c The character to look for
* @param indices Pointer to the int array to store the indices in
* @return The length of the indices array, 0 if it's not found, or an error code (less than 0)
*     if there was an error while allocating memory
*/
int indicesOf(const char* str, char c, int** indices);


#ifdef __cplusplus
};
#endif // __cplusplus

#ifdef BMD_HEADERS_ONLY
	#ifndef BMD_STRUTIL_IMPL
		#define BMD_STRUTIL_IMPL
#include "errors.h"

#include <string.h>
#include <stdlib.h>

int copyStr_s(char* dest, const char* src, int max)
{
	int size = 0;
	char character;
	const char* cpySrc = src;
	do
	{
		if (size >= max)
		{
			dbgprinterr("Error: String \"%s\" exceeds max length allowed (%i)\n",
						cpySrc, max);
			return BMD_ERROR_EXCEEDS_LENGTH;
		}
		character = *src++;
		dest[ size ] = character;
		size++;
	} while (character);

	return size;
}

int copyStr(char* dest, const char* src)
{
	int size = 0;
	char character;
	do
	{
		character = *src++;
		dest[ size ] = character;
		size++;
	} while (character);

	return size;
}

int copyStrDynamic_s(char** dest, const char* src, int max)
{
	int bufferSize = sizeof(char) + strlen(src);
	char* buffer = VOID_TO_CHAR malloc(bufferSize);
	if (!buffer) return BMD_ERROR_INVALID_MEMORY_ALLOCATION;
	int size = copyStr_s(buffer, src, max);
	*dest = buffer;
	return size;
}

int copyStrDynamic(char** dest, const char* src)
{
	int bufferSize = sizeof(char) + strlen(src);
	char* buffer = VOID_TO_CHAR malloc(bufferSize);
	if (!buffer) return BMD_ERROR_INVALID_MEMORY_ALLOCATION;
	int size = copyStr(buffer, src);
	*dest = buffer;
	return size;
}

int concatStr(char* orig, const char* add)
{
	if (!orig) return BMD_ERROR_NULL_STRING;
	if (!add) return BMD_ERROR_NULL_STRING;
	while (*orig)
		orig++;

	while (*add)
	{
		*orig = *add;
		add++;
		orig++;
	}

	*orig = NULL_TERM;
	return BMD_NO_ERROR;
}

int concatStr_r(char* orig, const char* add, int start, int stop)
{
	if (!orig) return BMD_ERROR_NULL_STRING;
	if (!add) return BMD_ERROR_NULL_STRING;
	while (*orig)
		orig++;
	int i = 0;
	while (*add)
	{
		if (i <= start)
		{
			add++;
			i++;
			continue;
		}
		*orig = *add;
		add++;
		orig++;
		i++;
		if (i > stop) break;
	}

	*orig = NULL_TERM;
	return BMD_NO_ERROR;
}

int concatStr_to(char* orig, const char* add, int stop)
{
	return concatStr_r(orig, add, -1, stop);
}

int concatStrDynamic(char** orig, const char* add)
{
	if (!orig) return BMD_ERROR_NULL_STRING;
	if (!add) return BMD_ERROR_NULL_STRING;
	int bufferSize = NULL_TERM_SIZE + strlen(*orig) + strlen(add);
	char* buffer = VOID_TO_CHAR malloc(bufferSize);
	if (!buffer) return BMD_ERROR_INVALID_MEMORY_ALLOCATION;
	int error = BMD_NO_ERROR;
	if (strlen(*orig) > 0)
		copyStr(buffer, *orig);
	error = concatStr(buffer, add);
	if (!error) *orig = buffer;
	return error;
}


int concatStrDynamic_r(char** orig, const char* add, int start, int stop)
{
	if (!orig) return BMD_ERROR_NULL_STRING;
	if (!add) return BMD_ERROR_NULL_STRING;
	int bufferSize = NULL_TERM_SIZE + strlen(*orig) + strlen(add);
	char* buffer = VOID_TO_CHAR malloc(bufferSize);
	if (!buffer) return BMD_ERROR_INVALID_MEMORY_ALLOCATION;
	int error = BMD_NO_ERROR;
	if (strlen(*orig) > 0)
		copyStr(buffer, *orig);
	error = concatStr_r(buffer, add, start, stop);
	if (!error) *orig = buffer;
	return error;
}


int concatStrDynamic_to(char** orig, const char* add, int stop)
{
	if (!orig) return BMD_ERROR_NULL_STRING;
	if (!add) return BMD_ERROR_NULL_STRING;
	int bufferSize = NULL_TERM_SIZE + strlen(*orig) + strlen(add);
	char* buffer = VOID_TO_CHAR malloc(bufferSize);
	if (!buffer) return BMD_ERROR_INVALID_MEMORY_ALLOCATION;
	int error = BMD_NO_ERROR;
	if (strlen(*orig) > 0)
		copyStr(buffer, *orig);
	error = concatStr_to(buffer, add, stop);
	if (!error) *orig = buffer;
	return error;
}

char* substr(const char* str, int start, int stop)
{
	if (!str)
	{
		dbgprinterr("Error: Tried to get substring from a null string\n");
		return NULL;
	}
	int length = strlen(str);
	if (start < 0 || stop > length || start >= length || stop <= 0)
	{
		dbgprinterr("Error: Index out of bounds when trying to get a substring\n"
					"Ensure start >= 0 and stop < str length (%i). Given start: %i, stop: %i\n",
					length, start, stop);
		return NULL;
	}

	char* ptr = VOID_TO_CHAR malloc(stop - start + sizeof(char)); // + char size to account for \0
	int c;
	for (c = 0; c < (stop - start); c++)
	{
		*(ptr + c) = *(str + start);
		str++;
	}
	*(ptr + c) = NULL_TERM;
	return ptr;
}

char* substrFrom(const char* str, int start)
{
	if (!str)
	{
		dbgprinterr("Error: Attempted to capture a substring from a null string\n");
		return NULL;
	}

	return substr(str, start, strlen(str));
}

int indexOf(const char* str, char c)
{
	const char* ptr = strchr(str, c);
	int ret = (int) (ptr - str);
	if (ret == -1) return BMD_ERROR_CHAR_NOT_IN_STRING;
	return ret;
}

int lastIndexOf(const char* str, char c)
{
	const char* ptr = strrchr(str, c);
	int ret = (int) (ptr - str);
	if (ret == -1) return BMD_ERROR_CHAR_NOT_IN_STRING;
	return ret;
}


int indicesOf(const char* str, char c, int** indices)
{
	if (!str) return BMD_ERROR_NULL_STRING;
	int n = 0;
	int* ret = VOID_TO_INT malloc(strlen(str) * sizeof(int));
	if (!ret) return BMD_ERROR_INVALID_MEMORY_ALLOCATION;
	for (int i = 0; i < strlen(str); i++)
	{
		if (str[ i ] == c)
		{
			ret[ n ] = i;
			n++;
		}
	}
	*indices = ret;
	return n;
}
	#endif
#endif


#endif //BMD_STRUTIL_H
