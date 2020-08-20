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
 * File Name: modelrenderer.cpp
 * Date File Created: 8/15/2020 at 11:05 PM
 * Author: Matt
 */

#include "mage/graphics/modelrenderer.h"
#include "mage/core/assetmanager.h"
#include <utility>

mage::BasicModelRenderer::BasicModelRenderer(SharedPtr<mage::Model> model)
{
    m_model = std::move(model);
    m_shader = AssetManager::getShader("./assets/shaders/basic");
}

void mage::BasicModelRenderer::render(const mage::RenderEngine* renderEngine)
{
    m_shader.enable();
    m_shader.setUniformMatf("model", getTransform().getTransformation());
    m_shader.setUniformMatf("projection", renderEngine->getCamera()->getProjectionMatrix());
    m_shader.setUniformMatf("view", renderEngine->getCamera()->getViewMatrix());
    postRender(renderEngine);
}

void mage::BasicModelRenderer::postRender(const mage::RenderEngine* renderEngine)
{
    renderEngine->renderModel(m_shader, *m_model);
    m_shader.disable();
}


mage::ModelRenderer::ModelRenderer(SharedPtr<mage::Model> model, vec3f lightPos) : BasicModelRenderer(std::move(model)),
                                                                                       m_lightPos(lightPos),
                                                                                       transposed(1)
{
    m_shader = AssetManager::getShader("./assets/shaders/lighting");
}

void mage::ModelRenderer::update(float delta)
{
}

void mage::ModelRenderer::postRender(const mage::RenderEngine* renderEngine)
{
    transposed = glm::transpose(
                        glm::inverse(getTransform().getTransformation()));
    m_shader.setUniformMatf("normalMatrix", transposed);
    m_shader.setUniform3f("light.ambient", vec3f(0.1f));
    m_shader.setUniform3f("light.diffuse", vec3f(1));
    m_shader.setUniform3f("light.specular", vec3f(0.5f));
    m_shader.setUniform3f("light.position", renderEngine->getCamera()->getPosition());
    m_shader.setUniform3f("light.direction", renderEngine->getCamera()->getFront());
    m_shader.setUniform1f("light.constant", 1.0f);
    m_shader.setUniform1f("light.linear", 0.07f);
    m_shader.setUniform1f("light.quadratic", 0.017f);
    m_shader.setUniform1f("light.cutoff", glm::cos(glm::radians(5.5f)));
    m_shader.setUniform1f("light.outerCutoff", glm::cos(glm::radians(12.5f)));
    m_shader.setUniform3f("viewPos", renderEngine->getCamera()->getPosition());


    renderEngine->renderModel(m_shader, *m_model);
    m_shader.disable();
}
