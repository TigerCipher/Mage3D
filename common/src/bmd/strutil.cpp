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

void concatStr(char* orig, char* add)
{
	while(*orig)
		orig++;

	while(*add)
	{
		*orig = *add;
		add++;
		orig++;
	}

	*orig = '\0';
}
