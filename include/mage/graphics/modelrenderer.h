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
 * File Name: ModelRenderer.h
 * Date File Created: 8/14/2020 at 10:19 PM
 * Author: Matt
 */

#ifndef MAGE3D_MODELRENDERER_H
#define MAGE3D_MODELRENDERER_H

#include <utility>

#include "mage3d_exported.h"
#include "mage/common.h"
#include "mage/core/gamecomponent.h"
#include "model.h"
#include "material.h"
#include "mage/core/camera.h"

namespace mage
{

    class BasicModelRenderer : public GameComponent
    {
    public:
        mage3d_EXPORT explicit BasicModelRenderer(SharedPtr<Model> model);
        mage3d_EXPORT void render(const RenderEngine* renderEngine) override;
    protected:

        virtual void postRender(const mage::RenderEngine* renderEngine);

        SharedPtr<Model> m_model;
        Shader m_shader;
    };

    class ModelRenderer : public BasicModelRenderer
    {
    public:

        mage3d_EXPORT ModelRenderer(SharedPtr<Model> model, vec3f lightPos);

        void update(float delta) override;

    protected:

        void postRender(const mage::RenderEngine* renderEngine) override;
        mat4f transposed;
        vec3f m_lightPos;
    };



}

#endif //MAGE3D_MODELRENDERER_H
