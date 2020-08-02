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
 * File Name: errors.h
 * Date File Created: 7/29/2020 at 11:03 PM
 * Author: Matt
 */

#ifndef BMD_ERRORS_H
#define BMD_ERRORS_H

#define BMD_NO_ERROR 0
#define BMD_OKAY BMD_NO_ERROR
#define BMD_OK BMD_OKAY
#define BMD_ERROR_NOT_YET_IMPLEMENTED -1
#define BMD_ERROR_NOT_SUPPORTED_BY_PLATFORM -2
#define BMD_ERROR_CHAR_NOT_IN_STRING -3
#define BMD_ERROR_OPEN_DIR -4
#define BMD_ERROR_CLOSE_DIR -5
#define BMD_ERROR_NEXT_FILE -6
#define BMD_ERROR_READ_FILE -7
#define BMD_ERROR_NULL_FILE -8
#define BMD_ERROR_FILE_NOT_FOUND -9
#define BMD_ERROR_FILE_TIME -10
#define BMD_ERROR_CHAR_NOT_FOUND -11
#define BMD_ERROR_NULL_STRING -12
#define BMD_ERROR_EXCEEDS_LENGTH -13
#define BMD_ERROR_INVALID_MEMORY_ALLOCATION -14

#endif //BMD_ERRORS_H
