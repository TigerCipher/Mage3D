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
#include "mage/common.h"
#include "mage/core/resourcemanager.h"
#include <map>
#include <string>
#include <ostream>


namespace mage
{

	struct AttribInfo
	{
		char name[32];
		int location;
		GLenum type;
	};

	class ShaderData : public ResourceManager
    {
    public:
        mage3d_EXPORT ShaderData(const char* vertPath, const char* fragPath);
        mage3d_EXPORT virtual ~ShaderData();

        mage3d_EXPORT void bind() const;
        mage3d_EXPORT void unbind();

        [[nodiscard]] mage3d_EXPORT inline uint getId() const { return m_id; }

	    ShaderData(const ShaderData& rhs) = delete;
	    ShaderData& operator=(const ShaderData& rhs) = delete;
	    ShaderData(ShaderData&& rhs) = delete;
	    ShaderData& operator=(ShaderData&& rhs) = delete;

        friend std::ostream& operator<<(std::ostream& os, const ShaderData& data);

    private:
        uint load(const char* vertSrc, const char* fragSrc);

        std::string preprocess(const std::string& fileName);

        uint m_id;
        std::string m_vertFile;
        std::string m_fragFile;
    };

	class Shader
	{
	public:
		mage3d_EXPORT Shader() = default;
		mage3d_EXPORT explicit Shader(const char* basePath);
		mage3d_EXPORT Shader(const char* vertPath, const char* fragPath);
		mage3d_EXPORT virtual ~Shader() = default;
		mage3d_EXPORT void destroy() const;

		mage3d_EXPORT void enable() const;
		mage3d_EXPORT void disable() const;

		mage3d_EXPORT void setUniform1i(const GLchar* name, int value) const;
		mage3d_EXPORT void setUniform1f(const GLchar* name, float value) const;
		mage3d_EXPORT void setUniform2f(const GLchar* name, const glm::vec2& value) const;
		mage3d_EXPORT void setUniform3f(const GLchar* name, const glm::vec3& value) const;
		mage3d_EXPORT void setUniform4f(const GLchar* name, const glm::vec4& value) const;
		mage3d_EXPORT void setUniformMatf(const GLchar* name, const glm::mat4& value) const;

		mage3d_EXPORT int getAttribLocation(const char* attribName);
		mage3d_EXPORT int getAttribLocation(const char* attribName) const;

		mage3d_EXPORT Shader(const Shader& rhs);
		mage3d_EXPORT Shader& operator=(const Shader& rhs);

        friend std::ostream& operator<<(std::ostream& os, const Shader& shader);


    protected:
	private:
		GLint getLocation(const GLchar* name) const;
		ShaderData* m_shaderData{};
		std::string m_keyName;

		static std::map<std::string, ShaderData*> shaderMap;
	};

}
#endif //MAGE3D_SHADER_H
