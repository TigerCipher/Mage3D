/*
 * Blutilities
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

/**
* Overwrites the data in the destination string with the data from the src string
* <p>
* This is safe only in the sense that it will not copy past the given max, but due to dynamic allocation being used,
* when your code is done with the copied string (dest), be sure to call free(dest)!
* </p>
* @param dest The string to overwrite. Must be a fixed string, not dynamic.
* @param src The source string to copy to the destination
* @param max The maximum allowed length to copy for use cases where this can effect safety
* @return The size of the new destination string
*/
#define copyStr_s(dest, src, max) __copyStr(dest, src, max, __FILE__, __LINE__)





/**
* Overwrites the data in the destination string with the data from the src string
* @param dest The string to overwrite
* @param src The source string to copy to the destination
* @param max The maximum allowed length to copy for use cases where this can effect safety
* @return The size of the new destination string or -1 if the src is longer than the given max
*/
#define copyStrDynamic_s(dest, src, max) _copyStrDynamic_safe(dest, src, max, __FILE__, __LINE__)

/**
* Captures a sub string contained within str ranging from [start - (stop - 1)]
* @param str The string containing the desired sub string
* @param start The beginning index of the sub string, inclusive
* @param stop The ending index of the sub string, exclusive
* @return The substring, or NULL if either index were out of bounds
*/
#define substr(str, start, stop) substr_(str, start, stop, __FILE__, __LINE__)

/**
* Overwrites the data in the destination string with the data from the src string. The copyStr_s macro should be used.
* @param dest The string to overwrite. Must be a fixed string, not dynamic.
* @param src The source string to copy to the destination
* @param max The maximum allowed length to copy for use cases where this can effect safety
* @param file The file the function was executed in
* @param line The line number where the function was used
* @return The size of the new destination string or -1 if the src is longer than the given max
*/
extern int copyStr_safe_(char* dest, const char* src, int max, const char* file, int line);

/**
* Overwrites the data in the destination string with the data from the src string
* @param dest The string to overwrite. Must be a fixed string, not dynamic.
* @param src The source string to copy to the destination
* @return The size of the new destination string
*/
extern int copyStr(char* dest, const char* src);


/**
* Overwrites the data in the destination string with the data from the src string. The copyStrDynamic_s macro should be used.
* <p>
* This is safe only in the sense that it will not copy past the given max, but due to dynamic allocation being used,
* when your code is done with the copied string (dest), be sure to call free(dest)!
* </p>
* @param dest The string to overwrite
* @param src The source string to copy to the destination
* @param max The maximum allowed length to copy for use cases where this can effect safety
* @param file The file the function was executed in
* @param line The line number where the function was used
* @return The size of the new destination string or -1 if the src is longer than the given max
*/
extern int copyStrDynamic_safe_(char*& dest, const char* src, int max, const char* file, int line);


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
extern int copyStrDynamic(char*& dest, const char* src);

/**
* Concatenates two C-Style strings with unknown lengths
* @param orig The original C-String
* @param add The C-String to append to orig
*/
extern void concatStr(char* orig, char* add);

/**
* Concatenates two C-Style strings with unknown lengths
* @param orig The original C-String
* @param add The C-String to append to orig
* @param stop The index to stop concatenating at. Must be less than the length of add
*/
extern void concatStr(char* orig, char* add, int stop);

/**
* Captures a sub string contained within str ranging from [start - (stop - 1)]. Use substr macro
* @param str The string containing the desired sub string
* @param start The beginning index of the sub string, inclusive
* @param stop The ending index of the sub string, exclusive
* @param file The file this function was called in
* @param line The line number of the file this function was called in
* @return The substring, or NULL if either index were out of bounds
*/
extern char* substr_(const char* str, int start, int stop, const char* file, int line);

#endif //BMD_STRUTIL_H
