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
#include "mage/core/util.h"
#include "mage/core/engine.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

std::map<std::string, mage::ShaderData*> mage::Shader::shaderMap;

mage::Shader::Shader(const char* basePath)
{
    char fragPath[MAX_PATH_LENGTH] = { };
    char vertPath[MAX_PATH_LENGTH] = { };
    copyStr(vertPath, basePath);
    copyStr(fragPath, basePath);
    concatStr(vertPath, ".vert");
    concatStr(fragPath, ".frag");
    m_keyName = std::string(vertPath);
    m_keyName += ";";
    m_keyName += fragPath;
    std::map<std::string, ShaderData*>::const_iterator it = shaderMap.find(m_keyName);
    if (it != shaderMap.end())
    {
        m_shaderData = it->second;
        m_shaderData->addReference();
        //m_shaderData("Ref count %i", m_textureData->getResourceCount());
    } else
    {
        m_shaderData = new ShaderData(vertPath, fragPath);
        shaderMap.insert(std::pair<std::string, ShaderData*>(m_keyName, m_shaderData));
    }
}

mage::Shader::Shader(const char* vertPath, const char* fragPath)
{
    m_keyName = std::string(vertPath);
    m_keyName += ";";
    m_keyName += fragPath;
    std::map<std::string, ShaderData*>::const_iterator it = shaderMap.find(m_keyName);
    if (it != shaderMap.end())
    {
        m_shaderData = it->second;
        m_shaderData->addReference();
        //m_shaderData("Ref count %i", m_textureData->getResourceCount());
    } else
    {
        m_shaderData = new ShaderData(vertPath, fragPath);
        shaderMap.insert(std::pair<std::string, ShaderData*>(m_keyName, m_shaderData));
    }
}

mage::Shader::~Shader()
{
    destroy();
}


void mage::Shader::setUniform1i(const GLchar* name, int value) const
{
    glUniform1i(getLocation(name), value);
}

void mage::Shader::setUniform1f(const GLchar* name, float value) const
{
    glUniform1f(getLocation(name), value);
}

void mage::Shader::setUniform2f(const GLchar* name, const glm::vec2& value) const
{
    glUniform2f(getLocation(name), value.x, value.y);
}

void mage::Shader::setUniform3f(const GLchar* name, const glm::vec3& value) const
{
    glUniform3f(getLocation(name), value.x, value.y, value.z);
}

void mage::Shader::setUniform4f(const GLchar* name, const glm::vec4& value) const
{
    glUniform4f(getLocation(name), value.x, value.y, value.z, value.w);
}

void mage::Shader::setUniformMatf(const GLchar* name, const glm::mat4& value) const
{
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

GLint mage::Shader::getLocation(const GLchar* name) const
{
    return glGetUniformLocation(m_shaderData->getId(), name);
}

void mage::Shader::enable() const
{
    m_shaderData->bind();
}

void mage::Shader::disable() const
{
    m_shaderData->unbind();
}

[[maybe_unused]] int mage::Shader::getAttribLocation(const char* attribName)
{
    return glGetAttribLocation(m_shaderData->getId(), attribName);
}

[[maybe_unused]] int mage::Shader::getAttribLocation(const char* attribName) const
{
    return glGetAttribLocation(m_shaderData->getId(), attribName);
}

void mage::Shader::destroy() const
{
    if (m_shaderData && m_shaderData->removeReference())
    {
        if (m_keyName.length() > 0)
            shaderMap.erase(m_keyName);
        delete m_shaderData;
    }
}

mage::Shader::Shader(const mage::Shader& rhs)
{
    rhs.m_shaderData->addReference();
    m_shaderData = rhs.m_shaderData;
    m_keyName = rhs.m_keyName;
}

mage::Shader& mage::Shader::operator=(const mage::Shader& rhs)
{
    rhs.m_shaderData->addReference();
    m_shaderData = rhs.m_shaderData;
    m_keyName = rhs.m_keyName;
    return *this;
}


mage::ShaderData::ShaderData(const char* vertPath, const char* fragPath) :
        m_vertFile(vertPath),
        m_fragFile(fragPath)
{
    m_id = load(vertPath, fragPath);
}

mage::ShaderData::~ShaderData()
{
    LOG_INFO("Unloading shader file {}", m_vertFile);
    LOG_INFO("Unloading shader file {}", m_fragFile);
    LOG_TRACE("Deleting shader program ID: {}", m_id);
    glDeleteProgram(m_id);
}

void mage::ShaderData::bind() const
{
    glUseProgram(m_id);
}

void mage::ShaderData::unbind()
{
    glUseProgram(0);
}

uint mage::ShaderData::load(const char* vertFile, const char* fragFile)
{
    std::string vsrc = preprocess(vertFile);
    std::string fsrc = preprocess(fragFile);
    const char* vertSrc = vsrc.c_str();
    const char* fragSrc = fsrc.c_str();
    GLuint program = glCreateProgram();
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);


    LOG_TRACE("Loading shader file {}", vertFile);
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
        LOG_CRITICAL("Error while compiling vertex shader [{}]. Error: {}", vertFile,
                     &error[ 0 ]);
        glDeleteShader(vertex);
        Engine::instance()->stop();
        return 0;
    }

    LOG_TRACE("Loading shader file {}", fragFile);
    glShaderSource(fragment, 1, &fragSrc, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint len;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> error(len);
        glGetShaderInfoLog(fragment, len, &len, &error[ 0 ]);
        LOG_CRITICAL("Error while compiling fragment shader [{}]. Error: {}", fragFile,
                     &error[ 0 ]);
        glDeleteShader(fragment);
        Engine::instance()->stop();
        return 0;
    }

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glValidateProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, (int*) &success);
    if (!success)
    {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> error(len);
        glGetProgramInfoLog(program, len, &len, &error[ 0 ]);
        LOG_CRITICAL("Error while linking shader program. Error: {}", &error[ 0 ]);
        glDeleteProgram(program);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        Engine::instance()->stop();
        return 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

std::string mage::ShaderData::preprocess(const std::string& fileName)
{
    static const std::string INCLUDE_KEY = "#include";
    std::string line;
    std::string output;
    std::ifstream file;
    file.open(fileName);

    if (file.is_open())
    {
        while (file.good())
        {
            getline(file, line);
            if (line.find(INCLUDE_KEY) == std::string::npos)
                output.append(line + "\n");
            else
            {
                std::string includeFileName = Util::split(line, ' ')[ 1 ];
                includeFileName = includeFileName.substr(1, includeFileName.length() - 2);
                std::string ap = preprocess("./assets/shaders/" + includeFileName);
                output.append(ap + "\n");
            }
        }
    } else
    {
        LOG_CRITICAL("Failed to process shader {}", fileName);
        Engine::instance()->stop();
    }


    return output;
}
