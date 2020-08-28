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
 * File Name: skybox.h
 * Date File Created: 8/28/2020 at 4:42 PM
 * Author: Matt
 */

#ifndef MAGE3D_SKYBOX_H
#define MAGE3D_SKYBOX_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "renderengine.h"


namespace mage
{
    class Skybox
    {
    public:
        mage3d_EXPORT explicit Skybox(list<std::string> faces);
        mage3d_EXPORT virtual ~Skybox();

        mage3d_EXPORT void render(const RenderEngine* renderEngine);

    protected:
    private:
        Shader m_shader;
        Mesh* m_mesh;
        TextureCube* m_texture;
    };

}


#endif //MAGE3D_SKYBOX_H
