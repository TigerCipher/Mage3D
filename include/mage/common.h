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

#define BMD_PROFILE 1
#include <bmd/core.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

#include "mage3d_exported.h"
#include "core/log.h"

// Rename some of the glm types, for quality of life sake
using vec2f = glm::vec2;
using vec3f = glm::vec3;
using vec4f = glm::vec4;
using mat3f = glm::mat3;
using mat4f = glm::mat4;
using vec2i = glm::ivec2;
using vec3i = glm::ivec3;
using vec4i = glm::ivec4;
using quaternion = glm::quat;

#define SIZE_OF_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

mage3d_EXPORT const char* getGlErrorString(GLenum err);


#ifndef MAGE_DEBUG
    #define MAGE_DEBUG 1
#endif
#ifndef MAGE_VERBOSE
    #define MAGE_VERBOSE 0
#endif


#define checkGlError(er) er = glGetError();\
if(er != GL_NO_ERROR) \
fprintf(stderr, "OpenGL Error 0x%x (%i): %s\nOccurred in %s at line %i\n", er, er, getGlErrorString(er), __FILE__, __LINE__)


#if MAGE_DEBUG
    #define DBGPRINT(fmt_, ...) printf(fmt_"\n", __VA_ARGS__)
    #define DBGPRINT_ERR(fmt_, ...) fprintf(stderr, fmt_"\n", __VA_ARGS__)
#else
    #define DBGPRINT(...)
    #define DBGPRINT_ERR(...)
#endif // MAGE_DEBUG

#if MAGE_VERBOSE
    #define VERBOSE_PRINT_ERR(fmt_, ...) fprintf(stderr, fmt_"\n", ##__VA_ARGS__)
#else
    #define VERBOSE_PRINT_ERR(...)
#endif // MAGE_VERBOSE

#endif //MAGE3D_COMMON_H
