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
    m_shader.setUniform3f("directionLight.ambient", vec3f(0.05f));
    m_shader.setUniform3f("directionLight.diffuse", vec3f(0.4f));
    m_shader.setUniform3f("directionLight.specular", vec3f(0.5f));
    m_shader.setUniform3f("directionLight.direction", vec3f(-0.2f, -1, -0.3f));

    m_shader.setUniform3f("pointLights[0].ambient", vec3f(0.05f));
    m_shader.setUniform3f("pointLights[0].diffuse", vec3f(0.3f));
    m_shader.setUniform3f("pointLights[0].specular", vec3f(0.5f));
    m_shader.setUniform3f("pointLights[0].position", m_lightPos);
    m_shader.setUniform1f("pointLights[0].constant", 1.0f);
    m_shader.setUniform1f("pointLights[0].linear", 0.07f);
    m_shader.setUniform1f("pointLights[0].quadratic", 0.017f);

    m_shader.setUniform3f("pointLights[1].ambient", vec3f(0.05f));
    m_shader.setUniform3f("pointLights[1].diffuse", vec3f(0.5f, 0.5f, 0));
    m_shader.setUniform3f("pointLights[1].specular", vec3f(0.5f));
    m_shader.setUniform3f("pointLights[1].position", m_lightPos + vec3f(10, 0, 0));
    m_shader.setUniform1f("pointLights[1].constant", 1.0f);
    m_shader.setUniform1f("pointLights[1].linear", 0.07f);
    m_shader.setUniform1f("pointLights[1].quadratic", 0.017f);

    m_shader.setUniform3f("pointLights[2].ambient", vec3f(0.05f));
    m_shader.setUniform3f("pointLights[2].diffuse", vec3f(0, 0.5f, 0.7f));
    m_shader.setUniform3f("pointLights[2].specular", vec3f(0.5f));
    m_shader.setUniform3f("pointLights[2].position", m_lightPos + vec3f(20, 0, 0));
    m_shader.setUniform1f("pointLights[2].constant", 1.0f);
    m_shader.setUniform1f("pointLights[2].linear", 0.07f);
    m_shader.setUniform1f("pointLights[2].quadratic", 0.017f);

    m_shader.setUniform3f("pointLights[3].ambient", vec3f(0.05f));
    m_shader.setUniform3f("pointLights[3].diffuse", vec3f(0, 0.7f, 0));
    m_shader.setUniform3f("pointLights[3].specular", vec3f(0.5f));
    m_shader.setUniform3f("pointLights[3].position", m_lightPos + vec3f(30, 0, 0));
    m_shader.setUniform1f("pointLights[3].constant", 1.0f);
    m_shader.setUniform1f("pointLights[3].linear", 0.07f);
    m_shader.setUniform1f("pointLights[3].quadratic", 0.017f);

    m_shader.setUniform3f("spotLight.ambient", vec3f(0.05f));
    m_shader.setUniform3f("spotLight.diffuse", vec3f(0, 0.5f, 0.5f));
    m_shader.setUniform3f("spotLight.specular", vec3f(0.7f));
    m_shader.setUniform3f("spotLight.position", renderEngine->getCamera()->getPosition());
    m_shader.setUniform3f("spotLight.direction", renderEngine->getCamera()->getFront());
    m_shader.setUniform1f("spotLight.constant", 1.0f);
    m_shader.setUniform1f("spotLight.linear", 0.07f);
    m_shader.setUniform1f("spotLight.quadratic", 0.017f);
    m_shader.setUniform1f("spotLight.cutoff", glm::cos(glm::radians(5.5f)));
    m_shader.setUniform1f("spotLight.outerCutoff", glm::cos(glm::radians(12.5f)));

    m_shader.setUniform3f("viewPos", renderEngine->getCamera()->getPosition());


    renderEngine->renderModel(m_shader, *m_model);
    m_shader.disable();
}
