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
 * File Name: quadrenderer.h
 * Date File Created: 8/22/2020 at 9:05 PM
 * Author: Matt
 */

#ifndef MAGE3D_QUADRENDERER_H
#define MAGE3D_QUADRENDERER_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "shader.h"
#include "mesh.h"
#include "mage/core/assetmanager.h"
#include "mage/core/gamecomponent.h"

namespace mage
{
    class QuadRenderer : public GameComponent
    {
    public:
        mage3d_EXPORT QuadRenderer(const std::string& textureName);
        mage3d_EXPORT ~QuadRenderer() override;

        mage3d_EXPORT void render(const RenderEngine* renderEngine) override;

    protected:
    private:
        Mesh* m_quad;
        Texture m_texture;
        Shader m_shader;
    };

}


#endif //MAGE3D_QUADRENDERER_H
