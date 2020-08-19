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
 * File Name: renderengine.h
 * Date File Created: 8/15/2020 at 9:58 PM
 * Author: Matt
 */

#ifndef MAGE3D_RENDERENGINE_H
#define MAGE3D_RENDERENGINE_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "shader.h"
#include "mesh.h"
#include "material.h"
#include "model.h"
#include <map>

namespace mage
{

    class Camera;

    class RenderEngine
    {
    public:
        mage3d_EXPORT RenderEngine();
        mage3d_EXPORT virtual ~RenderEngine();

        mage3d_EXPORT void registerShader(const Shader* shader, const std::string& shaderName) const;
        mage3d_EXPORT const Shader* getShader(const std::string& shader) const;

        mage3d_EXPORT void renderMesh(const Shader* shader, Mesh* mesh) const;
        mage3d_EXPORT void renderMesh(const Shader* shader, Mesh* mesh, const Material* material) const;
        mage3d_EXPORT void
        renderModel(const Shader* shader, Model* model) const;

        mage3d_EXPORT inline void addCamera(Camera* camera) const { m_camera = camera; }
        mage3d_EXPORT inline Camera* getCamera() const { return m_camera; }


        //static const Shader* BASIC_SHADER;
        //static const Shader* LIGHTING_SHADER;
        //static const Texture* DEFAULT_DIFFUSE_TEXTURE;
        //static const Texture* DEFAULT_SPECULAR_TEXTURE;
        //static const Texture* DEFAULT_EMISSION_TEXTURE;
    protected:
    private:
        mutable std::map<std::string, const Shader*> shaderMap;
        mutable Camera* m_camera{};
    };

}


#endif //MAGE3D_RENDERENGINE_H
