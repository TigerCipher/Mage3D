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
 * File Name: shader.h
 * Date File Created: 7/23/2020 at 4:40 PM
 * Author: Matt
 */

#ifndef MAGE3D_SHADER_H
#define MAGE3D_SHADER_H

#include "mage3d_exported.h"
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


namespace mage
{
	class Shader
	{
	public:
		mage3d_EXPORT Shader(const char* basePath);
		mage3d_EXPORT ~Shader();

		mage3d_EXPORT void enable() const;
		mage3d_EXPORT void disable() const;

		mage3d_EXPORT void setUniform1i(const GLchar* name, int value);
		mage3d_EXPORT void setUniform1f(const GLchar* name, float value);
		mage3d_EXPORT void setUniform2f(const GLchar* name, const glm::vec2& value);
		mage3d_EXPORT void setUniform3f(const GLchar* name, const glm::vec3& value);
		mage3d_EXPORT void setUniform4f(const GLchar* name, const glm::vec4& value);
		mage3d_EXPORT void setUniformMatf(const GLchar* name, const glm::mat4& value);
	protected:
	private:
		GLuint load(const char* vertexFile, const char* fragmentFile);
		GLint getLocation(const GLchar* name);

		GLuint m_id;
		const char* m_basePath;
	};

}
#endif //MAGE3D_SHADER_H
