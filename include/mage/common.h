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
 * File Name: common.h
 * Date File Created: 7/28/2020 at 8:44 PM
 * Author: Matt
 */

#ifndef MAGE3D_COMMON_H
#define MAGE3D_COMMON_H

#include <bmd/types.h>

#include <glm/glm.hpp>

// Rename some of the glm types, again for quality of life sake
using vec2f = glm::vec2;
using vec3f = glm::vec3;
using vec4f = glm::vec4;
using mat4f = glm::mat4;
using vec2i = glm::ivec2;
using vec3i = glm::ivec3;
using vec4i = glm::ivec4;


#define SIZE_OF_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))


#define checkGlError() if(glGetError() != GL_NO_ERROR) \
fprintf(stderr, "OpenGL Error (%i):\n%s\nOccured in %s at line %i\n", glGetError(), glGetString(glGetError()), __FILE__, __LINE__);

#define DBGPRINT(fmt_, ...) printf(fmt_"\n", ##__VA_ARGS__)
#define DBGPRINT_ERR(fmt_, ...) fprintf(stderr, fmt_"\n", ##__VA_ARGS__)
#ifdef MAGE_VERBOSE
	#define VERBOSE_PRINT_ERR(fmt_, ...) fprintf(stderr, fmt_"\n", ##__VA_ARGS__)
#else
	#define VERBOSE_PRINT_ERR(...)
#endif // MAGE_VERBOSE

#endif //MAGE3D_COMMON_H
