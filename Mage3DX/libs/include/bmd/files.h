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
 * File Name: files.h
 * Date File Created: 7/29/2020 at 10:16 PM
 * Author: Matt
 */

// Goal of this file is to create a hopefully platform independent
// fast and useful set of file system related functions
// TODO: Might be a good idea to make this use C only functions and headers and make it its own small library?
// More experienced in C++ than flat out C, so C++ only functionality might appear
// Ultimate goal is ideally C only, but for quality of life sake that probably won't happen

#ifndef BMD_FILES_H
#define BMD_FILES_H


#include "types.h"
#include "platform.h"
#include "common.h"


#include <string.h> // deprecated in favor of cstring, but this will allow future portability to C


// Max char array values
// Might get rid of and support dynamic lengths later, but I think on the OS level there are limits to
// these lengths as well? Needs further research
// I *BELIEVE* the max length for individual components is 255, making max absolute path limited at ~32,000?
// Seems rather extreme.
// Also belive that on windows the MAX_PATH is 259 for files and 248 for folders, whether thats the full path
// or individual components I'm not sure. Needs more research
// Also seems that if prefixed by \\?\ then the max path becomes 32,767
// https://docs.microsoft.com/en-us/windows/win32/fileio/naming-a-file#:~:text=In%20the%20Windows%20API%20(with,and%20a%20terminating%20null%20character.

// For now stick with these, as I don't think I've come across any with more characters
#define MAX_PATH_LENGTH 1024
#define MAX_FILENAME_LENGTH 256
#define MAX_EXT_LENGTH 32

struct FSFile;
struct FSDir;
struct FSTime;
// C compatibility
typedef struct FSFile fs_file;
typedef struct FSDir fs_dir;
typedef struct FSTime fs_time;

// Quality of life typedefs
typedef fs_file t_file;
typedef fs_dir t_dir;
typedef fs_time t_time;

typedef fs_file file_t;
typedef fs_dir dir_t;
//typedef fs_time time_t; // time_t is already defined else where

typedef fs_file file_;
typedef fs_dir dir_;
typedef fs_time time_;

typedef void (* fs_callback)(file_t* file, void* data);
typedef fs_callback filesystem_callback;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
* Captures and sets the extension of the given file
* @param file The file to capture the extension for
* @return A pointer to the extension (file::ext)
*/
const char* getExt(file_t* file);

/**
* Checks if the given file extension matches the given extension
* @param file The file to check
* @param ext The extension to check against
* @return zero if false, non-zero if true
*/
int doesFileHaveExt(file_t* file, const char* ext);

/**
* Checks if the file from the given path exists or not
* @param path The path of the file to check
* @return zero if false, non-zero if true
*/
int doesFileExist(const char* path);

/**
* Fills out the data in the fs_file struct, excluding fs_file::contents. To get the contents of the file,
* use either loadFileAndReadContents(dir_t* dir, file_t* file) readFile(const char* file) or readFileContents(file_t* file)
* @param dir The fs_dir struct containing the file
* @param file The file to have the data filled
* @return zero if there was no error, non-zero if there was an error
*/
int loadFileFromDir(dir_t* dir, file_t* file);

/**
* Fills out the data in the fs_file struct, excluding fs_file::contents. To get the contents of the file,
* use either loadFileAndReadContents(dir_t* dir, file_t* file) readFile(const char* file) or readFileContents(file_t* file)
*
* @param dirPath The path of the directory to load to search for the file
* @param fileName The name of the file to load
* @param file The fs_file struct to load
* @return zero if there was no error, non-zero if there was an error
*/
int loadFile_(const char* dirPath, const char* fileName, file_t* file);

/**
* Fills out the data in the fs_file struct, excluding fs_file::contents. To get the contents of the file,
* use either loadFileAndReadContents(dir_t* dir, file_t* file) readFile(const char* file) or readFileContents(file_t* file)
*
* @param filePath The path of the file to load
* @param file The fs_file struct to load
* @return zero if there was no error, non-zero if there was an error
*/
int loadFile(const char* filePath, file_t* file);

/**
* Fills out the data in the fs_file struct. For a more lightweight function,
* see loadFile(dir_t* dir, file_t* file)
* @param dir The fs_dir struct containing the file
* @param file The file to have the data filled
* @return zero if there was no error, non-zero if there was an error
*/
int loadFileFromDirAndReadContents(dir_t* dir, file_t* file);

/**
* Fills out the data in the fs_file struct. For a more lightweight function,
* see loadFile(dir_t* dir, file_t* file)
* @param dir The directory containing the file, i.e for ./dirA/dirB/test.txt it would be ./dirA/dirB
* @param fileName The name of the file, i.e test.txt
* @param file The fs_file struct to store the data in
* @return 0 if no error, non zero if error
*/
int loadFileAndReadContents_(const char* dir, const char* fileName, file_t* file);

/**
* Fills out the data in the fs_file struct. For a more lightweight function,
* see loadFile(dir_t* dir, file_t* file)
* @param filePath The path of the file, i.e ./dirA/dirB/test.txt
* @param file The fs_file struct to store the data in
* @return 0 if no error, non zero if error
*/
int loadFileAndReadContents(const char* filePath, file_t* file);

/**
* Unloads a file from BMD's file system. Frees up any allocated memory and closes the FILE stream if need be
* @param file The file to unload
* @return 0 if no error, non 0 if error
*/
int unloadFile(file_t* file);

/**
* Flushes the file stream contained in the FSFile struct
* @param file The file to flush
* @return 0 if no error, non 0 if error
*/
int flushFile(file_t* file);

/**
* Takes an already filled out fs_file struct and reads the contents of the file
* @param file The file to read the contents of and fill the file::contents
* @return zero if there was no error, non-zero if there was an error
*/
int readFileContents(file_t* file);

int writeFile(const char* file, const char* data, const char* mode);
int writeToFile(file_t* file, const char* data, const char* mode);

/**
* Reads the contents of a file
* @param file The path to the file to read
* @param data A pointer to the char array to store the file contents in. The data will have memory allocated,
*     so be sure to call free(data)
* @return 0 if no error, non zero if error
*/
int readFile(const char* file, char** data);


/**
* Opens and fills out the contents of the given fs_dir struct
* @param dir The fs_dir struct to fill with data
* @param path The directory path to open
* @return zero if there was no error, non-zero if there was an error
*/
int openDir(dir_t* dir, const char* path);

/**
* Closes the directory
* @param dir The fs_dir struct to close
* @return zero if there was no error, non-zero if there was an error
*/
int closeDir(dir_t* dir);

/**
* Creates the given directory if it does not exist
* @param path The directory to create
* @return zero if no error, non-zero if there was an error
*/
int createDir(const char* path);

/**
* From an opened fs_dir, this grabs the next file
* @param dir The opened fs_dir (see openDir(dir_t* dir) )
* @return zero if there was no error, non-zero if there was an error
*/
int nextFile(dir_t* dir);

/**
* Performs the given callback function on every file found in the given directory
*
* @param dirPath The directory to traverse
* @param callback The function to perform on the files
* @param userData
* @return zero if there was no error, non-zero if there was an error
*/
int traverse(const char* dirPath, fs_callback callback, void* userData);

/**
* Performs the given callback function on every file found in the given directory and it's sub directories
*
* @param dirPath The directory to traverse
* @param callback The function to perform on the files
* @param userData
* @return zero if there was no error, non-zero if there was an error
*/
int traverse_r(const char* dirPath, fs_callback callback, void* userData);

/**
* Retrieves the time when the file at the given path was created
* @param path The path of the file
* @param time A pointer to store the time in
* @return zero if there was no error, non-zero if there was an error
*/
int getCreationTime(const char* path, fs_time* time);

/**
* Retrieves the time when the file was created
* @param file The fs_file struct
* @param time A pointer to store the time in
* @return zero if there was no error, non-zero if there was an error
*/
int getCreationTimeOfFile(file_t* file, fs_time* time);

/**
* Retrieves the time when the file at the given path was last modified
* @param path The path of the file
* @param time A pointer to store the time in
* @return zero if there was no error, non-zero if there was an error
*/
int getLastModifiedTime(const char* path, fs_time* time);

/**
* Retrieves the time when the file was last modified
* @param file The fs_file struct
* @param time A pointer to store the time in
* @return zero if there was no error, non-zero if there was an error
*/
int getLastModifiedTimeOfFile(file_t* file, fs_time* time);


/**
* Compares two given times
* @param a The first time
* @param b The second time
* @return If a happened before b, -1. If b happened before a, 1. If they are equal, 0
*/
int compareTimes(fs_time* a, fs_time* b);

#ifdef __cplusplus
};
#endif // __cplusplus


// TODO: Add function to clear the file
// TODO: On the loadFile and loadFileAndReadContents, maybe add callback so it can load the data as an image
// Like, in Mage3D's case, callback(file_t* file, stuff) { file->contents = SOIL_load_image.... } ?



#ifdef OS_WINDOWS

//#include <Windows.h>

/**
* Represents a file in the BMD file system api
*/
struct FSFile
{
	/** A string containing the full path of the file (not necessarily absolute) */
	char path[MAX_PATH_LENGTH];

	/** A string containing the name of the file. The path is stripped away */
	char name[MAX_FILENAME_LENGTH];

	/** A string containing the extension of the file */
	char ext[MAX_EXT_LENGTH];

	/** 1 if the file is a directory, 0 otherwise */
	int isDir;

	/** 1 if the file is a non-directory normal file, 0 otherwise */
	int isFile;

	/** The size of the file, includes the null terminating character */
	size_t size;

	/** The contents of the file. Null until the file is read */
	char* contents;

	/** Flushing and closing the file stream can be costly, this is simply to keep track of a file stream so that
	* opening, closing, flushing, etc are only done when actually needed or desired*/
	FILE* outStream;

	FILE* inStream;
};


/**
* Represents a directory in the bmd file system api
*/
struct FSDir
{
	/** A string containing the full path of the file (not necessarily absolute) */
	char path[MAX_PATH_LENGTH];

	/** 1 if the directory has another file it has not yet encountered, 0 otherwise */
	int hasNext;

	// Windows information
	HANDLE handle;
	WIN32_FIND_DATAA fdata;
};

struct FSTime
{
	/** The Windows FILETIME structure for a file */
	FILETIME time;

	/** A string containing formatted output of the file time */
	char time_str[50];
};

#else
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

struct FSFile
{
	char path[MAX_PATH_LENGTH];
	char name[MAX_FILENAME_LENGTH];
	char ext[MAX_EXT_LENGTH];
	int isDir;
	int isFile;
	int size;
	char* contents;
	/** Flushing and closing the file stream can be costly, this is simply to keep track of a file stream so that
	* opening, closing, flushing, etc are only done when actually needed or desired*/
	FILE* outStream;
	FILE* inStream;
	struct stat info;
};

struct FSDir
{
	char path[MAX_PATH_LENGTH];
	int hasNext;
	DIR* dir;
	struct dirent* entry;
};

struct FSTime
{
	time_t time;
	char time_str[50];
};

#endif // OS_WINDOWS


#ifdef BMD_HEADERS_ONLY
	#ifndef BMD_FILES_IMPL
		#define BMD_FILES_IMPL

#include "errors.h"
#include "strutil.h"

#include <Windows.h>


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

	if (period) copyStr_s(file->ext, period, MAX_EXT_LENGTH);
	else file->ext[ 0 ] = 0;
	return file->ext;
}

int doesFileHaveExt(file_t* file, const char* ext)
{
	// strcmp returns 0 if they match, so !strcmp returns 0 when it's not 0 and 1 when it is 0
	return !strcmp(file->ext, ext);
}


int loadFile_(const char* dirPath, const char* fileName, file_t* file)
{
	file->inStream = NULL;
	file->outStream = NULL;
	file->contents = 0;
	int error = BMD_NO_ERROR;
	dir_t dir;
	error = openDir(&dir, dirPath);
	if (error) return error;
	file_t temp;
	while (dir.hasNext)
	{
		error = loadFileFromDir(&dir, &temp);
		if (error) return error;
		if (temp.isFile && strcmp(temp.name, fileName) == 0)
		{
			copyStr_s(file->path, temp.path, MAX_PATH_LENGTH);
			copyStr_s(file->name, temp.name, MAX_FILENAME_LENGTH);
			copyStr_s(file->ext, temp.ext, MAX_EXT_LENGTH);
			file->isDir = temp.isDir;
			file->isFile = temp.isFile;
			file->size = temp.size;
			dbgprintln("Loading file [path=%s, name=%s, dir=%i, file=%i, size=%zu]", file->path, file->name,
					   file->isDir, file->isFile, file->size);
			closeDir(&dir);
			return error;
		}
		error = nextFile(&dir);
		if (error) return error;
	}

	closeDir(&dir);
	return BMD_ERROR_FILE_NOT_FOUND;
}

int loadFile(const char* filePath, file_t* file)
{
	if (!doesFileExist(filePath)) return BMD_ERROR_FILE_NOT_FOUND;
	int lastSlashIndex = lastIndexOf(filePath, '/');
	if (lastSlashIndex < 0)
		lastSlashIndex = lastIndexOf(filePath, '\\');
	if (lastSlashIndex < 0)
		return lastSlashIndex;
	char* dirName = substr(filePath, 0, lastSlashIndex);
	char* fileName = substrFrom(filePath, lastSlashIndex + 1);

	return loadFile_(dirName, fileName, file);
}

int readFile(const char* file, char** data)
{
	if (!doesFileExist(file)) return BMD_ERROR_FILE_NOT_FOUND;
	FILE* f;
	int error = fopen_s(&f, file, "rt");
	checkErrorMsg(error, "Could not open file %s\n", file);
	fseek(f, 0, SEEK_END);
	ulong len = ftell(f);
	if(len == 0) return BMD_ERROR_EMPTY_FILE;
	char* buffer = VOID_TO_CHAR malloc(len + 1);
	if (!buffer)
	{
		checkErrorMsg(BMD_ERROR_INVALID_MEMORY_ALLOCATION,
					  "Failed to allocate memory while reading file %s\n", file);
	}
	memset(buffer, 0, len + 1);
	fseek(f, 0, SEEK_SET);
	fread(buffer, 1, len, f);
	*data = buffer;
	error = fclose(f);
	checkErrorMsg(error, "Failed to close file %s\n", file);
	return BMD_NO_ERROR;
}

int loadFileAndReadContents_(const char* dir, const char* fileName, file_t* file)
{
	file->inStream = NULL;
	file->outStream = NULL;
	file->contents = 0;
	int error = BMD_NO_ERROR;
	dir_t dirt;
	error = openDir(&dirt, dir);
	if (error) return error;
	file_t temp;
	while (dirt.hasNext)
	{
		error = loadFileFromDirAndReadContents(&dirt, &temp);
		if (error) return error;
		if (temp.isFile && strcmp(temp.name, fileName) == 0)
		{
			copyStr_s(file->path, temp.path, MAX_PATH_LENGTH);
			copyStr_s(file->name, temp.name, MAX_FILENAME_LENGTH);
			copyStr_s(file->ext, temp.ext, MAX_EXT_LENGTH);
			file->isDir = temp.isDir;
			file->isFile = temp.isFile;
			file->size = temp.size;
			file->contents = temp.contents;
			closeDir(&dirt);
			return error;
		}
		error = nextFile(&dirt);
		if (error) return error;
	}
	closeDir(&dirt);
	return BMD_ERROR_FILE_NOT_FOUND;
}

int loadFileAndReadContents(const char* filePath, file_t* file)
{
	if(!doesFileExist(filePath)) return BMD_ERROR_FILE_NOT_FOUND;
	int lastSlashIndex = lastIndexOf(filePath, '/');
	if (lastSlashIndex < 0)
		lastSlashIndex = lastIndexOf(filePath, '\\');
	if (lastSlashIndex < 0)
		return lastSlashIndex;
	char* dirName = substr(filePath, 0, lastSlashIndex);
	char* fileName = substrFrom(filePath, lastSlashIndex + 1);

	return loadFileAndReadContents_(dirName, fileName, file);
}

int unloadFile(file_t* file)
{
	if (!file) return BMD_ERROR_NULL_FILE;
	if (file->contents)
	{
		free(file->contents);
		file->contents = NULL;
	}
	if (file->outStream)
	{
		int err = fclose(file->outStream);
		checkErrorMsg(err, "Error: Failed to close file out stream\n");
		file->outStream = NULL;
	}

	if (file->inStream)
	{
		int err = fclose(file->inStream);
		checkErrorMsg(err, "Error: Failed to close file in stream\n");
		file->inStream = NULL;
	}
	return BMD_NO_ERROR;
}


int flushFile(file_t* file)
{
	if (!file) return BMD_ERROR_NULL_FILE;
	if (file->outStream)
	{
		int err = fflush(file->outStream);
		checkErrorMsg(err, "Error: Failed to flush file out stream\n");
	}
	return BMD_NO_ERROR;
}

int readFileContents(file_t* file)
{
	if (!file) return BMD_ERROR_NULL_FILE;
	if (!doesFileExist(file->path)) return BMD_ERROR_FILE_NOT_FOUND;
	if (file->size <= 0) return BMD_ERROR_NULL_FILE;
	if (!file->inStream)
	{
		fopen_s(&file->inStream, file->path, "rt");
	}
	//fseek(f, 0, SEEK_END);
	//ulong len = ftell(f);
	char* data = VOID_TO_CHAR malloc(file->size);
	memset(data, 0, file->size);
	fread(data, 1, file->size - 1, file->inStream);
	//fclose(f);
	file->contents = data;
	if (!file->contents) return BMD_ERROR_READ_FILE;
	return BMD_NO_ERROR;
}

int writeFile(const char* file, const char* data, const char* mode)
{
	FILE* f;
	fopen_s(&f, file, mode);
	fwrite(data, 1, strlen(data), f);
	fclose(f);
	return BMD_NO_ERROR;
}

int writeToFile(file_t* file, const char* data, const char* mode)
{
	if (!file) return BMD_ERROR_NULL_FILE;
	if (!file->isFile) return BMD_ERROR_NOT_A_FILE;
	if (!file->outStream)
		fopen_s(&file->outStream, file->path, mode);
	fwrite(data, 1, strlen(data), file->outStream);
	fseek(file->outStream, 0, SEEK_END);
	ulong len = ftell(file->outStream);
	//fclose(f); // TODO This alone costs roughly 1000 to 2000 ms
	file->size = len + 1;
	return BMD_NO_ERROR;
}

int traverse(const char* dirPath, fs_callback callback, void* userData)
{
	dir_t dir;
	int error = openDir(&dir, dirPath);
	checkError(error);

	while (dir.hasNext)
	{
		file_t file;
		error = loadFileFromDir(&dir, &file);
		checkError(error);

		if (file.isFile)
			callback(&file, userData);
		error = nextFile(&dir);
		checkError(error);
	}
	error = closeDir(&dir);
	checkError(error);
	return BMD_NO_ERROR;
}

int traverse_r(const char* dirPath, fs_callback callback, void* userData)
{
	dir_t dir;
	int error = openDir(&dir, dirPath);
	checkError(error);

	while (dir.hasNext)
	{
		file_t file;
		error = loadFileFromDir(&dir, &file);
		checkError(error);
		if (file.isDir && file.name[ 0 ] != '.')
		{
			char path[MAX_PATH_LENGTH];
			int size = copyStr_s(path, dirPath, MAX_PATH_LENGTH);
			if (size < 0)
			{
				dbgprintln("Failed to copy directory path string while traversing");
				return size; // will be BMD_ERROR_EXCEEDS_LENGTH
			}
			error = concatStr(path, "/");
			checkError(error);
			error = concatStr(path, file.name);
			checkError(error);
			error = traverse_r(path, callback, userData);
			checkError(error);
		}

		if (file.isFile)
			callback(&file, userData);
		error = nextFile(&dir);
		checkError(error);
	}
	error = closeDir(&dir);
	checkError(error);
	return BMD_NO_ERROR;
}

int getCreationTimeOfFile(file_t* file, fs_time* time)
{
	if (!file) return BMD_ERROR_NULL_FILE;
	return getCreationTime(file->path, time);
}

int getLastModifiedTimeOfFile(file_t* file, fs_time* time)
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

int loadFileFromDir(dir_t* dir, file_t* file)
{
			BMD_ASSERT(dir->handle != INVALID_HANDLE_VALUE);
	file->inStream = NULL;
	file->outStream = NULL;
	file->contents = 0;
	int n = 0;
	int error = BMD_NO_ERROR;
	char* dirPath = dir->path;
	char* filePath = file->path;
	n = copyStr_s(filePath, dirPath, MAX_PATH_LENGTH);
	checkErrorMsg(n < 0, "Failed to set file path");
	error = concatStr(filePath, "/");
	checkErrorMsg(error, "Failed to add / to file path");

	char* dirName = dir->fdata.cFileName;
	char* fileName = file->name;

	n = copyStr_s(fileName, dirName, MAX_FILENAME_LENGTH);
	checkErrorMsg(n < 0, "Failed to set file name");

	error = concatStr(filePath, fileName);
	checkErrorMsg(error, "Failed to concatenate filepath and filename");

	file->size = ((size_t) dir->fdata.nFileSizeHigh * (MAXDWORD + 1)) + (size_t) dir->fdata.nFileSizeLow;
	dbgprintln("File size: %llu (for file %s)",
			   ((size_t) dir->fdata.nFileSizeHigh * (MAXDWORD + 1)) + (size_t) dir->fdata.nFileSizeLow, fileName);
	getExt(file);
	file->isDir = (dir->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	file->isFile = (dir->fdata.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0 ||
				   !(dir->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

	return BMD_NO_ERROR;
}

int loadFileFromDirAndReadContents(dir_t* dir, file_t* file)
{
			BMD_ASSERT(dir->handle != INVALID_HANDLE_VALUE);
	file->inStream = NULL;
	file->outStream = NULL;
	file->contents = 0;
	int n = 0;
	int error = BMD_NO_ERROR;
	char* dirPath = dir->path;
	char* filePath = file->path;
	n = copyStr_s(filePath, dirPath, MAX_PATH_LENGTH);
	if (n < 0) return n;
	error = concatStr(filePath, "/");
	if (error) return error;

	char* dirName = dir->fdata.cFileName;
	char* fileName = file->name;

	n = copyStr_s(fileName, dirName, MAX_FILENAME_LENGTH);
	if (n < 0) return n;

	error = concatStr(filePath, fileName);
	if (error) return error;

	file->size = ((size_t) dir->fdata.nFileSizeHigh * (MAXDWORD + 1)) + (size_t) dir->fdata.nFileSizeLow;
	getExt(file);
	file->isDir = (dir->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	file->isFile = (dir->fdata.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0 ||
				   !(dir->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	if (file->isFile && !file->isDir)
		error = readFileContents(file);

	return error;
}

int openDir(dir_t* dir, const char* path)
{
	int n = 0;
	int error = BMD_NO_ERROR;
	n = copyStr_s(dir->path, path, MAX_PATH_LENGTH);
	if (n < 0) return n;
	error = concatStr(dir->path, "\\*");
	if (error) return error;

	dir->handle = FindFirstFileA(dir->path, &dir->fdata);
	dir->path[ n - 1 ] = 0;

	if (dir->handle == INVALID_HANDLE_VALUE)
	{
		dbgprinterr("Error: Could not open directory [%s] - %s", path, strerror(errno));

		// To be safe, lets mark the directory as closed
		closeDir(dir);
		//BMD_ASSERT(0);
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

int createDir(const char* path)
{
	int success = CreateDirectoryA(path, 0);
	if (success) return BMD_NO_ERROR;
	DWORD err = GetLastError();
	if (err == ERROR_PATH_NOT_FOUND)
		return BMD_ERROR_PATH_NOT_FOUND;
	else if (err != ERROR_ALREADY_EXISTS)
		return BMD_ERROR_CREATE_DIR;
	return BMD_NO_ERROR;
}

int nextFile(dir_t* dir)
{
			BMD_ASSERT(dir->hasNext);

	if (!FindNextFileA(dir->handle, &dir->fdata))
	{
		dir->hasNext = 0;
		DWORD err = GetLastError();
		int retErr = err == ERROR_SUCCESS || err == ERROR_NO_MORE_FILES;
				BMD_ASSERT(retErr);
		return retErr ? BMD_NO_ERROR : BMD_ERROR_NEXT_FILE;
	}

	return BMD_NO_ERROR;
}

int getCreationTime(const char* path, fs_time* time)
{
	if (!doesFileExist(path)) return BMD_ERROR_FILE_NOT_FOUND;
	time->time;
	WIN32_FILE_ATTRIBUTE_DATA info;
	if (GetFileAttributesExA(path, GetFileExInfoStandard, &info))
	{
		time->time = info.ftCreationTime;

		TIME_ZONE_INFORMATION tzi;
		int zone = GetTimeZoneInformation(&tzi);
		SYSTEMTIME utcTime;
		FileTimeToSystemTime(&(info.ftCreationTime), &utcTime);
		SYSTEMTIME local;
		SystemTimeToTzSpecificLocalTime(&tzi, &utcTime, &local);
		WCHAR* zoneName = 0;
		if (zone == TIME_ZONE_ID_STANDARD)
			zoneName = tzi.StandardName;
		else if (zone == TIME_ZONE_ID_DAYLIGHT)
			zoneName = tzi.DaylightName;
		// Going for format -> MM/dd/yyyy at hh:mm:ss
		// That's 22 characters, 23 if we include null terminating character
		//char* formatted = VOID_TO_CHAR malloc(23 * sizeof(char));
		sprintf(time->time_str, "%02d/%02d/%04d at %02d:%02d:%02d %ls", local.wMonth, local.wDay, local.wYear,
				local.wHour, local.wMinute, local.wSecond, zoneName);
		//time->time_str = formatted;
		return BMD_NO_ERROR;
	}

	return BMD_ERROR_FILE_TIME;
}

int getLastModifiedTime(const char* path, fs_time* time)
{
	if (!doesFileExist(path)) return BMD_ERROR_FILE_NOT_FOUND;
	time->time;
	WIN32_FILE_ATTRIBUTE_DATA info;
	if (GetFileAttributesExA(path, GetFileExInfoStandard, &info))
	{
		time->time = info.ftLastWriteTime;

		TIME_ZONE_INFORMATION tzi;
		int zone = GetTimeZoneInformation(&tzi);
		SYSTEMTIME utcTime;
		FileTimeToSystemTime(&(info.ftLastWriteTime), &utcTime);
		SYSTEMTIME local;
		SystemTimeToTzSpecificLocalTime(&tzi, &utcTime, &local);
		// Going for format -> MM/dd/yyyy at hh:mm:ss
		// That's 22 characters, 23 if we include null terminating character
		//char* formatted = VOID_TO_CHAR malloc(30 * sizeof(char));
		WCHAR* zoneName = 0;
		if (zone == TIME_ZONE_ID_STANDARD)
			zoneName = tzi.StandardName;
		else if (zone == TIME_ZONE_ID_DAYLIGHT)
			zoneName = tzi.DaylightName;

		sprintf(time->time_str, "%02d/%02d/%04d at %02d:%02d:%02d %ls", local.wMonth, local.wDay, local.wYear,
				local.wHour, local.wMinute, local.wSecond, zoneName);
		//time->time_str = formatted;
		return BMD_NO_ERROR;
	}

	return BMD_ERROR_FILE_TIME;
}

int compareTimes(fs_time* a, fs_time* b)
{
	return CompareFileTime(&a->time, &b->time);
}

#else

int doesFileExist(const char* path)
{
	return 0;
}

int loadFileFromDir(dir_t* dir, file_t* file)
{
	return BMD_ERROR_NOT_YET_IMPLEMENTED;
}

int openDir(dir_t* dir, const char* path)
{
	return BMD_ERROR_NOT_YET_IMPLEMENTED;
}

int closeDir(dir_t* dir)
{
	return BMD_ERROR_NOT_YET_IMPLEMENTED;
}

int createDir(const char* path)
{
	return BMD_ERROR_NOT_YET_IMPLEMENTED;
}

int nextFile(dir_t* dir)
{
	return BMD_ERROR_NOT_YET_IMPLEMENTED;
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
	//time->time_str = ctime(time->time);
}

int compareTimes(fs_time* a, fs_time* b)
{
	return BMD_ERROR_NOT_YET_IMPLEMENTED;
}

#endif // OS Check
	#endif
#endif


#endif //BMD_FILES_H
