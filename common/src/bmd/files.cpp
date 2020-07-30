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
 * File Name: files.cpp
 * Date File Created: 7/29/2020 at 10:19 PM
 * Author: Matt
 */

#include "bmd/files.h"
#include "bmd/errors.h"
#include "bmd/strutil.h"

int safeCopy_internal(char* dest, const char* src, int n, int max, const char* file, int line)
{
	int character;
	const char* copyOfSrc = src;
	do
	{
		if (n >= max)
		{
			if (!DEBUGGING) return BMD_ERROR_EXCEEDS_LENGTH;
			fprintf(stderr, "Error: String \"%s\" exceeds max length allowed (%i) on line %i of %s\n",
					copyOfSrc, max, line, file);
					FILES_ASSERT(0);
			return BMD_ERROR_EXCEEDS_LENGTH;
		}
		character = *src++;
		dest[ n ] = character;
		n++;
	} while (character);

	return n;
}

const char* getExt(file_t* file)
{
	char* cpyOfName = file->name;
	char* period = NULL;
	while (*cpyOfName++)
	{
		if (*cpyOfName == '.')
		{
			period = cpyOfName;
			break;
		}
	}

	if (period) safeCopy(file->ext, period, 0, MAX_EXT_LENGTH);
	else file->ext[ 0 ] = 0;
	return file->ext;
}

int doesFileHaveExt(file_t* file, const char* ext)
{
	// strcmp returns 0 if they match, so !strcmp returns 0 when it's not 0 and 1 when it is 0
	return !strcmp(file->ext, ext);
}

int loadFile(const char* dirPath, const char* fileName, file_t& file)
{
	int error = BMD_NO_ERROR;
	dir_t dir;
	error = openDir(&dir, dirPath);
	if (error) return error;
	file_t temp;
	while (dir.hasNext)
	{
		error = loadFile(&dir, &temp);
		if (error) return error;
		if (temp.isFile && strcmp(temp.name, fileName) == 0)
		{
			file = temp;
			closeDir(&dir);
			return error;
		}
		error = nextFile(&dir);
		if (error) return error;
	}
	closeDir(&dir);
	return BMD_ERROR_FILE_NOT_FOUND;
}

char* readFile(const char* file)
{
	if (!doesFileExist(file)) return NULL;
	FILE* f = fopen(file, "rt");
	fseek(f, 0, SEEK_END);
	ulong len = ftell(f);
	char* data = new char[len + 1];
	fseek(f, 0, SEEK_SET);
	fread(data, 1, len, f);
	return data;
}

int loadFileAndReadContents(dir_t* dir, file_t* file)
{
	int error = loadFile(dir, file);
	if (!error)
	{
		if (!doesFileExist(file->path)) return BMD_ERROR_FILE_NOT_FOUND;
		file->contents = readFile(file->path);
		if (!file->contents) return BMD_ERROR_READ_FILE;
		return BMD_NO_ERROR;
	}
	return error;
}

int readFileContents(file_t* file)
{
	if (!file) return BMD_ERROR_NULL_FILE;
	if (!doesFileExist(file->path)) return BMD_ERROR_FILE_NOT_FOUND;
	file->contents = readFile(file->path);
	if (!file->contents) return BMD_ERROR_READ_FILE;
	return BMD_NO_ERROR;
}

int getCreationTime(file_t* file, fs_time* time)
{
	if (!file) return BMD_ERROR_NULL_FILE;
	return getCreationTime(file->path, time);
}

int getLastModifiedTime(file_t* file, fs_time* time)
{
	if (!file) return BMD_ERROR_NULL_FILE;
	return getLastModifiedTime(file->path, time);
}

#ifdef OS_WINDOWS

int doesFileExist(const char* path)
{
	WIN32_FILE_ATTRIBUTE_DATA temp;
	return GetFileAttributesExA(path, GetFileExInfoStandard, &temp);
}

int loadFile(dir_t* dir, file_t* file)
{
			FILES_ASSERT(dir->handle != INVALID_HANDLE_VALUE);
	int n = 0;
	char* dirPath = dir->path;
	char* filePath = file->path;

	n = safeCopy(filePath, dirPath, 0, MAX_PATH_LENGTH);
	if (n == BMD_ERROR_EXCEEDS_LENGTH) return n;
	n = safeCopy(filePath, "/", n - 1, MAX_PATH_LENGTH);
	if (n == BMD_ERROR_EXCEEDS_LENGTH) return n;

	char* dirName = dir->fdata.cFileName;
	char* fileName = file->name;

	int errCheck = 0;
	errCheck = safeCopy(fileName, dirName, 0, MAX_FILENAME_LENGTH);
	if (errCheck == BMD_ERROR_EXCEEDS_LENGTH) return errCheck;
	errCheck = safeCopy(filePath, fileName, n - 1, MAX_PATH_LENGTH);
	if (errCheck == BMD_ERROR_EXCEEDS_LENGTH) return errCheck;

	file->size = ((size_t) dir->fdata.nFileSizeHigh * (MAXDWORD + 1)) + (size_t) dir->fdata.nFileSizeLow;
	getExt(file);
	file->isDir = (dir->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	file->isFile = (dir->fdata.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0 ||
				   !(dir->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

	return BMD_NO_ERROR;
}

int openDir(dir_t* dir, const char* path)
{
	int n = 0;
	n = safeCopy(dir->path, path, 0, MAX_PATH_LENGTH);
	if (n == BMD_ERROR_EXCEEDS_LENGTH) return n;
	n = safeCopy(dir->path, "\\*", n - 1, MAX_PATH_LENGTH);
	if (n == BMD_ERROR_EXCEEDS_LENGTH) return n;

	dir->handle = FindFirstFileA(dir->path, &dir->fdata);
	dir->path[ n - 3 ] = 0;

	if (dir->handle == INVALID_HANDLE_VALUE)
	{
		if (DEBUGGING)
			fprintf(stderr, "Error: Could not open directory [%s] - %s", path, strerror(errno));
		// To be safe, lets mark the directory as closed
		closeDir(dir);
				FILES_ASSERT(0);
		return BMD_ERROR_OPEN_DIR;
	}

	dir->hasNext = 1;
	return BMD_NO_ERROR;
}


int closeDir(dir_t* dir)
{
	dir->path[ 0 ] = 0;
	dir->hasNext = 0;
	if (dir->handle != INVALID_HANDLE_VALUE)
		FindClose(dir->handle);
	else return BMD_ERROR_CLOSE_DIR;
	return BMD_NO_ERROR;
}

int nextFile(dir_t* dir)
{
			FILES_ASSERT(dir->hasNext);

	if (!FindNextFileA(dir->handle, &dir->fdata))
	{
		dir->hasNext = 0;
		DWORD err = GetLastError();
		int retErr = err == ERROR_SUCCESS || err == ERROR_NO_MORE_FILES;
				FILES_ASSERT(retErr);
		return retErr ? BMD_NO_ERROR : BMD_ERROR_NEXT_FILE;
	}

	return BMD_NO_ERROR;
}

int getCreationTime(const char* path, fs_time* time)
{
	if (!doesFileExist(path)) return BMD_ERROR_FILE_NOT_FOUND;
	time->time = { 0 };
	WIN32_FILE_ATTRIBUTE_DATA info;
	if (GetFileAttributesExA(path, GetFileExInfoStandard, &info))
	{
		time->time = info.ftCreationTime;
		SYSTEMTIME local = { 0 };
		FileTimeToSystemTime(&(info.ftCreationTime), &local);
		//GetLocalTime(&local); // changes it to the current local time
		// Going for format -> MM/dd/yyyy at hh:mm:ss
		// That's 22 characters, 23 if we include null terminating character
		char* formatted = (char*) malloc(23 * sizeof(char));
		sprintf(formatted, "%02d/%02d/%04d at %02d:%02d:%02d", local.wMonth, local.wDay, local.wYear,
				local.wHour, local.wMinute, local.wSecond);
		time->time_str = formatted;
		return BMD_NO_ERROR;
	}

	return BMD_ERROR_FILE_TIME;
}

int getLastModifiedTime(const char* path, fs_time* time)
{
	if (!doesFileExist(path)) return BMD_ERROR_FILE_NOT_FOUND;
	time->time = { 0 };
	WIN32_FILE_ATTRIBUTE_DATA info;
	if (GetFileAttributesExA(path, GetFileExInfoStandard, &info))
	{
		time->time = info.ftLastWriteTime;
		SYSTEMTIME local = { 0 };
		FileTimeToSystemTime(&(info.ftLastWriteTime), &local);
		//GetLocalTime(&local); // changes it to the current local time
		// Going for format -> MM/dd/yyyy at hh:mm:ss
		// That's 22 characters, 23 if we include null terminating character
		char* formatted = (char*) malloc(23 * sizeof(char));
		sprintf(formatted, "%02d/%02d/%04d at %02d:%02d:%02d", local.wMonth, local.wDay, local.wYear,
				local.wHour, local.wMinute, local.wSecond);
		time->time_str = formatted;
		return BMD_NO_ERROR;
	}

	return BMD_ERROR_FILE_TIME;
}

#else

int doesFileExist(const char* path)
{

}

int readFile(dir_t* dir, file_t* file)
{

}

int openDir(dir_t* dir, const char* path)
{

}

int closeDir(dir_t* dir)
{

}

int nextFile(dir_t* dir)
{

}

int getCreationTime(const char* path, fs_time* time)
{
	return BMD_ERROR_NOT_SUPPORTED_BY_PLATFORM;
}

int getLastModifiedTime(const char* path, fs_time* time)
{
	struct stat info;
	if(stat(path, &info)) return BMD_ERROR_FILE_TIME;
	time->time = info.st_mtime;
	time->time_str = ctime(time->time);
}

#endif // OS Check



