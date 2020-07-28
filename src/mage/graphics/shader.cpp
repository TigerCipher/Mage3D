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
 * File Name: shader.cpp
 * Date File Created: 7/23/2020 at 4:40 PM
 * Author: Matt
 */

#include "mage/graphics/shader.h"
#include "mage/core/file.h"
#include "mage/core/stdcolor.h"
#include <string>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

mage::Shader::Shader(const char* basePath) :
		m_basePath(basePath)
{
	std::string vertex(basePath);
	vertex.append(".vert");
	std::string frag(basePath);
	frag.append(".frag");

	m_id = load(vertex.c_str(), frag.c_str());
}

mage::Shader::~Shader()
{
	glDeleteProgram(m_id);
}

GLuint mage::Shader::load(const char* vertexFile, const char* fragmentFile)
{
	GLuint program = glCreateProgram();
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	File vert(vertexFile);
	File frag(fragmentFile);
	std::string vertSource = vert.read();
	std::string fragSource = frag.read();
	// Prevent dangling char pointers by separating this
	const char* vertSrc = vertSource.c_str();
	const char* fragSrc = fragSource.c_str();


	glShaderSource(vertex, 1, &vertSrc, NULL);
	glCompileShader(vertex);
	GLint success;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint len;
		glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &len);
		std::vector<char> error(len);
		glGetShaderInfoLog(vertex, len, &len, &error[ 0 ]);
		print(console::RED, "Error while compiling vertex shader [{}]\nError: {}\n", vertexFile, &error[ 0 ]);
		glDeleteShader(vertex);
		return 0;
	}

	glShaderSource(fragment, 1, &fragSrc, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint len;
		glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &len);
		std::vector<char> error(len);
		glGetShaderInfoLog(fragment, len, &len, &error[ 0 ]);
		print(console::RED, "Error while compiling fragment shader [{}]\nError: {}\n", fragmentFile, &error[ 0 ]);
		glDeleteShader(fragment);
		return 0;
	}

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glValidateProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, (int*)&success);
	if(!success)
	{
		GLint len = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		std::vector<char> error(len);
		glGetProgramInfoLog(program, len, &len, &error[0]);
		print(console::RED, "Error while linking shader program\nError: {}\n", &error[0]);
		glDeleteProgram(program);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return 0;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return program;
}

void mage::Shader::setUniform1i(const GLchar* name, int value)
{
	glUniform1i(getLocation(name), value);
}

void mage::Shader::setUniform1f(const GLchar* name, float value)
{
	glUniform1f(getLocation(name), value);
}

void mage::Shader::setUniform2f(const GLchar* name, const glm::vec2& value)
{
	glUniform2f(getLocation(name), value.x, value.y);
}

void mage::Shader::setUniform3f(const GLchar* name, const glm::vec3& value)
{
	glUniform3f(getLocation(name), value.x, value.y, value.z);
}

void mage::Shader::setUniform4f(const GLchar* name, const glm::vec4& value)
{
	glUniform4f(getLocation(name), value.x, value.y, value.z, value.w);
}

void mage::Shader::setUniformMatf(const GLchar* name, const glm::mat4& value)
{
	glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

GLint mage::Shader::getLocation(const GLchar* name)
{
	return glGetUniformLocation(m_id, name);
}

void mage::Shader::enable() const
{
	glUseProgram(m_id);
}

void mage::Shader::disable() const
{
	glUseProgram(0);
}

