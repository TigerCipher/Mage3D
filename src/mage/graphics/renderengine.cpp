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
 * File Name: renderengine.cpp
 * Date File Created: 8/15/2020 at 9:58 PM
 * Author: Matt
 */

#include "mage/graphics/renderengine.h"
#include "mage/core/display.h"
#include "mage/core/gameobject.h"
#include "mage/core/assetmanager.h"

//const mage::Shader* mage::RenderEngine::BASIC_SHADER;
//const mage::Shader* mage::RenderEngine::LIGHTING_SHADER;
//const mage::Texture* mage::RenderEngine::DEFAULT_DIFFUSE_TEXTURE;
//const mage::Texture* mage::RenderEngine::DEFAULT_SPECULAR_TEXTURE;
//const mage::Texture* mage::RenderEngine::DEFAULT_EMISSION_TEXTURE;

mage::RenderEngine::RenderEngine()
{
    //BASIC_SHADER = new Shader("./assets/shaders/basic");
    //LIGHTING_SHADER = new Shader("./assets/shaders/lighting");
    //DEFAULT_DIFFUSE_TEXTURE = new Texture(TEXTURE_DEFAULT_DIFFUSE, TEXTURE_DIFFUSE);
    //DEFAULT_SPECULAR_TEXTURE = new Texture(TEXTURE_DEFAULT_SPECULAR, TEXTURE_SPECULAR);
    //DEFAULT_EMISSION_TEXTURE = new Texture(TEXTURE_DEFAULT_EMISSION, TEXTURE_EMISSION);

    //registerShader(BASIC_SHADER, "basic");
    //registerShader(LIGHTING_SHADER, "lighting");

    // TODO some models might need different culling
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_target = new Texture(Display::getWidth(), Display::getHeight(), 0, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA,
                           GL_RGBA, false, GL_COLOR_ATTACHMENT0);

    list<float> quadVertices = {
            // positions   // texCoords
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
    };

    m_screenMesh = new Mesh(quadVertices);
    m_filterShader = AssetManager::getShader("screen");
}

mage::RenderEngine::~RenderEngine()
{
    delete m_target;
    delete m_screenMesh;
    //for (auto & iterator : shaderMap)
    //{
    //    delete iterator.second;
    //}
    //
    //delete DEFAULT_DIFFUSE_TEXTURE;
    //delete DEFAULT_SPECULAR_TEXTURE;
    //delete DEFAULT_EMISSION_TEXTURE;
}

void mage::RenderEngine::registerShader(const mage::Shader* shader, const std::string& shaderName) const
{
    shaderMap[ shaderName ] = shader;
}

const mage::Shader* mage::RenderEngine::getShader(const std::string& shader) const
{
    return shaderMap[ shader ];
}

void mage::RenderEngine::renderMesh(const mage::Shader& shader, Mesh* mesh) const
{
    GLenum er;
    mesh->enable();

    mesh->render();

    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    //glDisableVertexAttribArray(2);

    Mesh::disable();
}

void mage::RenderEngine::renderMesh(const mage::Shader& shader, mage::Mesh* mesh,
                                    const mage::Material& material) const
{
    //if(!material)
    //{
    //    renderMesh(shader, mesh);
    //    return;
    //}
    material.getDiffuse().enable(0);
    material.getSpecular().enable(1);
    material.getEmission().enable(2);

    shader.setUniform1i("material.texture_diffuse", 0);
    shader.setUniform1i("material.texture_specular", 1);
    shader.setUniform1i("material.texture_emission", 2);
    shader.setUniform1f("material.shininess", material.getShininess());

    renderMesh(shader, mesh);

    material.getDiffuse().disable(0);
    material.getSpecular().disable(1);
    material.getEmission().disable(2);
}

void mage::RenderEngine::renderModel(const mage::Shader& shader, const Model& model) const
{
    for (int i = 0; i < model.getMeshes().size(); i++)
    {
        renderMesh(shader, model.getMeshes()[ i ], model.getMaterial());
    }
}

void mage::RenderEngine::render(mage::GameObject* gameObject) const
{
    glEnable(GL_DEPTH_TEST);
    gameObject->renderAll(this);
    applyFilter(m_filterShader, *m_target);
}

void mage::RenderEngine::applyFilter(const mage::Shader& filter, const mage::Texture& src, Texture* dest) const
{
    assert(&src != dest);
    if(!dest)
        Display::bindAsRenderTarget();
    else dest->bindAsRenderTarget();
    filter.enable();
    m_screenMesh->enable_basic();
    src.enable(0);
    m_screenMesh->draw_basic();
    m_screenMesh->disable_basic();
    src.disable(0);
    filter.disable();
}

void mage::RenderEngine::applyFilter() const
{
    Display::bindAsRenderTarget();
    m_filterShader.enable();
    m_screenMesh->enable_basic();
    m_target->enable(0);
    m_screenMesh->draw_basic();
    m_screenMesh->disable_basic();
    m_target->disable(0);
    m_filterShader.disable();
}

void mage::RenderEngine::preRender() const
{
    m_target->bindAsRenderTarget();
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

