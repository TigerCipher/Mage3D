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


const mage::Shader* mage::RenderEngine::BASIC_SHADER;
const mage::Shader* mage::RenderEngine::LIGHTING_SHADER;
const mage::Texture* mage::RenderEngine::DEFAULT_DIFFUSE_TEXTURE;
const mage::Texture* mage::RenderEngine::DEFAULT_SPECULAR_TEXTURE;
const mage::Texture* mage::RenderEngine::DEFAULT_EMISSION_TEXTURE;

mage::RenderEngine::RenderEngine()
{
    BASIC_SHADER = new Shader("./assets/shaders/basic");
    LIGHTING_SHADER = new Shader("./assets/shaders/lighting");
    DEFAULT_DIFFUSE_TEXTURE = new Texture(TEXTURE_DEFAULT_DIFFUSE, TEXTURE_DIFFUSE);
    DEFAULT_SPECULAR_TEXTURE = new Texture(TEXTURE_DEFAULT_SPECULAR, TEXTURE_SPECULAR);
    DEFAULT_EMISSION_TEXTURE = new Texture(TEXTURE_DEFAULT_EMISSION, TEXTURE_EMISSION);

    registerShader(BASIC_SHADER, "basic");
    registerShader(LIGHTING_SHADER, "lighting");
}

mage::RenderEngine::~RenderEngine()
{
    for (auto & iterator : shaderMap)
    {
        delete iterator.second;
    }

    delete DEFAULT_DIFFUSE_TEXTURE;
    delete DEFAULT_SPECULAR_TEXTURE;
    delete DEFAULT_EMISSION_TEXTURE;
}

void mage::RenderEngine::registerShader(const mage::Shader* shader, const std::string& shaderName) const
{
    shaderMap[shaderName] = shader;
}

const mage::Shader* mage::RenderEngine::getShader(const std::string& shader) const
{
    return shaderMap[shader];
}

void mage::RenderEngine::renderMesh(const mage::Shader* shader, Mesh* mesh) const
{
    mesh->enable();
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(shader->getAttribLocation("position"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, pos));
    glVertexAttribPointer(shader->getAttribLocation("texCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, coords));
    glVertexAttribPointer(shader->getAttribLocation("normal"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, normal));
    glVertexAttribPointer(shader->getAttribLocation("color"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, color));
    mesh->render();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    mesh->disable();
}

void mage::RenderEngine::renderMesh(const mage::Shader* shader, mage::Mesh* mesh, const mage::Material* material) const
{
    if(!material)
    {
        renderMesh(shader, mesh);
        return;
    }

    const Texture* diffuse = material->diffuse ? material->diffuse : DEFAULT_DIFFUSE_TEXTURE;
    const Texture* specular = material->specular ? material->specular : DEFAULT_SPECULAR_TEXTURE;
    const Texture* emission = material->emission ? material->emission : DEFAULT_EMISSION_TEXTURE;
    diffuse->enable(0);
    specular->enable(1);
    emission->enable(2);

    shader->setUniform1i("material.texture_diffuse", 0);
    shader->setUniform1i("material.texture_specular", 1);
    shader->setUniform1i("material.texture_emission", 2);
    shader->setUniform1f("material.shininess", material->shininess);

    renderMesh(shader, mesh);

    diffuse->disable(0);
    specular->disable(1);
    emission->disable(2);
}

void mage::RenderEngine::renderModel(const mage::Shader* shader, Model* model, const mage::Material* material) const
{
    for(int i = 0; i < model->getMeshes().size(); i++)
    {
        renderMesh(shader, model->getMeshes()[i], material);
    }
}
