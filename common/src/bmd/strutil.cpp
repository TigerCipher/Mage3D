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
 * File Name: strutil.cpp
 * Date File Created: 7/30/2020 at 12:38 AM
 * Author: Matt
 */


#include "bmd/strutil.h"
#include "bmd/types.h"
#include "bmd/errors.h"
#include "bmd/common.h"

#include <string.h>

int copyStr_safe_(char* dest, const char* src, int max, const char* file, int line)
{
	int size = 0;
	int character;
	const char* cpySrc = src;
	do
	{
		if (size >= max)
		{
			if (DEBUGGING)
				fprintf(stderr, "Error: String \"%s\" exceeds max length allowed (%i) on line %i of %s\n",
						cpySrc, max, line, file);
					FILES_ASSERT(0);
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
	int character;
	do
	{
		character = *src++;
		dest[ size ] = character;
		size++;
	} while (character);

	return size;
}

int copyStrDynamic_safe_(char*& dest, const char* src, int max, const char* file, int line)
{
	int bufferSize = sizeof(char) + strlen(src);
	char* buffer = (char*) malloc(bufferSize);
	int size = copyStr_safe_(buffer, src, max, file, line);
	dest = buffer;
	return size;
}

int copyStrDynamic(char*& dest, const char* src)
{
	int bufferSize = sizeof(char) + strlen(src);
	char* buffer = (char*) malloc(bufferSize);
	int size = copyStr(buffer, src);
	dest = buffer;
	return size;
}

void concatStr(char* orig, char* add)
{
	while (*orig)
		orig++;

	while (*add)
	{
		*orig = *add;
		add++;
		orig++;
	}

	*orig = '\0';
}

void concatStr(char* orig, char* add, int stop)
{
	while (*orig)
		orig++;

	int i = 0;
	while (*add)
	{
		*orig = *add;
		add++;
		orig++;
		i++;
		if (i >= stop)
			break;
	}

	*orig = '\0';
}

char* substr_(const char* str, int start, int stop, const char* file, int line)
{
	if (!str)
	{
		if (DEBUGGING)
			fprintf(stderr, "Error: Tried to get substring from a null string in "
							"file %s at line %i", file,
					line);
		return NULL;
	}
	int length = strlen(str);
	if (start < 0 || stop >= length)
	{
		if (DEBUGGING)
			fprintf(stderr,
					"Error: Index out of bounds when trying to get a substring in file %s at line %i. "
					"Ensure start >= 0 and stop < str length",
					file, line);
		return NULL;
	}

	char* ptr = (char*) malloc(stop - start + sizeof(char)); // + char size to account for \0
	int c;
	for(c = 0; c < (stop - start); c++)
	{
		*(ptr + c) = *(str + start);
		str++;
	}
	*(ptr + c) = '\0';
	return ptr;
}
