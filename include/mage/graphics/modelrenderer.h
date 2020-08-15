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

    enum ShaderType
    {
        LIGHTING, LAMP
    };

    class ModelRenderer : public GameComponent
    {
    public:
        ModelRenderer(SharedPtr<Model> model, SharedPtr<Material> material, SharedPtr<Camera> camera,
                      vec3f lightPos, ShaderType type) :
                m_lightPos(lightPos),
                transposed(1),
                m_type(type)
        {
            m_model = std::move(model);
            m_material = std::move(material);
            m_camera = std::move(camera);
        }

        void update(float delta) override
        {
            if(m_type == LIGHTING)
                transposed = glm::transpose(
                        glm::inverse(getTransform().getTransformation() * m_camera->getViewMatrix()));
        }

        void render(Renderer* renderer, Shader* shader) override
        {
            shader->enable();
            shader->setUniformMatf("model", getTransform().getTransformation());
            shader->setUniformMatf("projection", m_camera->getProjectionMatrix());
            shader->setUniformMatf("view", m_camera->getViewMatrix());
            if (m_type == LIGHTING)
            {
                //transposed = glm::transpose(
                //        glm::inverse(getTransform().getTransformation() * m_camera->getViewMatrix()));
                shader->setUniformMatf("normalMatrix", transposed);
                shader->setUniform3f("light.ambient", vec3f(0.1f));
                shader->setUniform3f("light.diffuse", vec3f(0.5f));
                shader->setUniform3f("light.specular", vec3f(1));
                shader->setUniform3f("lightPos", m_lightPos);
                renderer->render(*shader, *m_model, *m_material);
            } else renderer->render(*shader, *m_model);

            shader->disable();
        }

    private:
        SharedPtr<Model> m_model;
        SharedPtr<Material> m_material;
        SharedPtr<Camera> m_camera;
        mat4f transposed;
        vec3f m_lightPos;
        ShaderType m_type;
    };
}

#endif //MAGE3D_MODELRENDERER_H
