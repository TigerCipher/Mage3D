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
 * File Name: quadrenderer.cpp
 * Date File Created: 8/22/2020 at 9:05 PM
 * Author: Matt
 */

#include "mage/graphics/quadrenderer.h"
#include "mage/core/camera.h"

mage::QuadRenderer::QuadRenderer(const std::string& textureName)
{
    list<Vertex> verts = {
            Vertex(Position(-1, -1, -1), Normal(), TexCoords(0, 0)),
            Vertex(Position(1, -1, -1), Normal(), TexCoords(0, 1)),
            Vertex(Position(1, -1, 1), Normal(), TexCoords(1, 1)),
            Vertex(Position(-1, -1, 1), Normal(), TexCoords(1, 0))
    };
    list<uint> ints = {
            0, 1, 2,
            2, 3, 0
    };

    m_quad = new Mesh(verts, ints);
    m_texture = AssetManager::getTexture(textureName);
    m_shader = AssetManager::getShader("transparent");
}

mage::QuadRenderer::~QuadRenderer()
{
    delete m_quad;
}


void mage::QuadRenderer::render(const mage::RenderEngine* renderEngine)
{
    m_shader.enable();
    m_texture.enable(0);
    m_shader.setUniformMatf("model", getTransform().getTransformation());
    m_shader.setUniformMatf("projection", renderEngine->getCamera()->getProjectionMatrix());
    m_shader.setUniformMatf("view", renderEngine->getCamera()->getViewMatrix());
    m_shader.setUniform1i("tex", 0);
    // Toggle face culling so both sides of the plane can be rendered
    glDisable(GL_CULL_FACE);
    renderEngine->renderMesh(m_shader, m_quad);
    glEnable(GL_CULL_FACE);
    m_texture.disable(0);
    m_shader.disable();
}
