/*
 * Mage3DX
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
 * File Name: DebugTools.h
 * Date File Created: 10/4/2020 at 8:11 PM
 * Author: Matt
 */
#pragma once

#include <bmd/types.h>

extern bool redirect_console();

extern bool release_console();

extern bool create_new_console(int16_f minLength);

extern void adjust_console_buffer(int16_f minLength);

extern bool attach_parent_console(int16_f minLength);

