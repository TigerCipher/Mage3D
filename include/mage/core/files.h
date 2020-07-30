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
 * File Name: files.h
 * Date File Created: 7/29/2020 at 5:33 PM
 * Author: Matt
 */

// Goal of this file is to create a hopefully platform independent
// fast and useful set of file system related functions
// TODO: Might be a good idea to make this use C only functions and headers and make it its own small library?
// More experienced in C++ than flat out C, so C++ only functionality might appear
// Ultimate goal is ideally C only, but for quality of life sake that probably won't happen

#ifndef MAGE3D_FILES_H
#define MAGE3D_FILES_H

// If I do make this file its own small little library (maybe header only library even)
// then these includes will either need to go (mage3d_exported related stuff completely)
// or platform.h and common.h will need to be part of the small library, and changed to conform with C standards
#include "mage3d_exported.h"
#include "mage/common.h"
#include "mage/core/platform.h"


#include <string.h> // deprecated in favor of cstring, but this will allow future portability to C

#ifndef DEBUGGING
// prefer if(DEBUGGING) over #ifdef _DEBUG so that even after code is compiled the debug flag can be changed
#define DEBUGGING 1
#endif // DEBUGGING

// If DEBUGGING is 1
#if DEBUGGING

#include <stdio.h>
#include <assert.h>
#include <errno.h>

#define FILES_ASSERT assert
#else
// If DEBUGGING is disabled (not 1) then don't use assert
#define FILES_ASSERT(...)
#endif // DEBUGGING

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

/**
* Captures and sets the extension of the given file
* @param file The file to capture the extension for
* @return A pointer to the extension (file::ext)
*/
extern mage3d_EXPORT const char* getExt(file_t* file);

/**
* Fills out the data in the fs_file struct, excluding fs_file::contents. To get the contents of the file,
* use either readFileContents(dir_t* dir, file_t* file) or readFile(const char* file)
* @param dir The fs_dir struct containing the file
* @param file The file to have the data filled
* @return zero if there was no error, non-zero if there was an error
*/
extern mage3d_EXPORT int readFile(dir_t* dir, file_t* file);

/**
* Fills out the data in the fs_file struct. For a more lightweight function,
* see readFile(dir_t* dir, file_t* file)
* @param dir The fs_dir struct containing the file
* @param file The file to have the data filled
* @return zero if there was no error, non-zero if there was an error
*/
extern mage3d_EXPORT int readFileContents(dir_t* dir, file_t* file);

/**
* Takes an already filled out fs_file struct and reads the contents of the file
* @param file The file to read the contents of and fill the file::contents
* @return zero if there was no error, non-zero if there was an error
*/
extern mage3d_EXPORT int readFileContents(file_t* file);

/**
* Reads the contents of a file
* @param file The full path to the file to read
* @return The contents of the file as a char pointer
*/
extern mage3d_EXPORT const char* readFile(const char* file);


/**
* Opens and fills out the contents of the given fs_dir struct
* @param dir The fs_dir struct to fill with data
* @param path The directory path to open
* @return zero if there was no error, non-zero if there was an error
*/
extern mage3d_EXPORT int openDir(dir_t* dir, const char* path);

/**
* Closes the directory
* @param dir The fs_dir struct to close
* @return zero if there was no error, non-zero if there was an error
*/
extern mage3d_EXPORT int closeDir(dir_t* dir);

/**
* From an opened fs_dir, this grabs the next file
* @param dir The opened fs_dir (see openDir(dir_t* dir) )
* @return zero if there was no error, non-zero if there was an error
*/
extern mage3d_EXPORT int nextFile(dir_t* dir);

/**
* Retrieves the time when the file at the given path was created
* @param path The path of the file
* @param time A pointer to store the time in
* @return zero if there was no error, non-zero if there was an error
*/
extern mage3d_EXPORT int getCreationTime(const char* path, fs_time* time);

/**
* Retrieves the time when the file was created
* @param file The fs_file struct
* @param time A pointer to store the time in
* @return zero if there was no error, non-zero if there was an error
*/
extern mage3d_EXPORT int getCreationTime(file_t* file, fs_time* time);

/**
* Retrieves the time when the file at the given path was last modified
* @param path The path of the file
* @param time A pointer to store the time in
* @return zero if there was no error, non-zero if there was an error
*/
extern mage3d_EXPORT int getLastModifiedTime(const char* path, fs_time* time);

/**
* Retrieves the time when the file was last modified
* @param file The fs_file struct
* @param time A pointer to store the time in
* @return zero if there was no error, non-zero if there was an error
*/
extern mage3d_EXPORT int getLastModifiedTime(file_t* file, fs_time* time);

// TODO: Add implementations
// TODO: Add functions to compare times
// TODO: Add functions to check states (i.e if a file exists or not)
// TODO: Add recursive functions to retrieve files in a directory and/or sub directory(ies)
// TODO: Add function to write to the file
// TODO: Add function to append to the file
// TODO: Add function to clear the file
// TODO: Add function to delete the file
// TODO: On the readFile and readFileContents, maybe add callback so it can load the data as an image
// Like, in Mage3D's case, callback(file_t* file, stuff) { file->contents = SOIL_load_image.... } ?
// TODO: Make a callback wrapper so capture can be used? i.e [&](stuff) {} ? Might need static_cast and c++ however



#ifdef OS_WINDOWS
#include <Windows.h>
struct FSFile
{
	char path[MAX_PATH_LENGTH];
	char name[MAX_FILENAME_LENGTH];
	char ext[MAX_EXT_LENGTH];
	int isDir;
	int isFile;
	size_t size;
	char* contents;
};

struct FSDir
{
    char path[MAX_PATH_LENGTH];
    int hasNext;
    HANDLE handle;
    WIN32_FIND_DATAA fdata;
};

struct FSTime
{
	FILETIME time;
	// TODO: Store formatted char* of the time?
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
	// TODO: Store formatted char* of the time?
};

#endif // OS_WINDOWS

#endif //MAGE3D_FILES_H
