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
 * File Name: platform.h
 * Date File Created: 7/29/2020 at 10:26 PM
 * Author: Matt
 */

#ifndef BMD_PLATFORM_H
#define BMD_PLATFORM_H

#if defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS) || defined(__WIN32__) || defined(__WINDOWS__) || defined(__TOS_WIN__)
#define OS_NAME "Windows"
	#if defined(WIN64) || defined(_WIN64)
		#define OS_WINDOWS_64
	#endif
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
