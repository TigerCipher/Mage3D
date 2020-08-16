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
 * File Name: renderer.cpp
 * Date File Created: 7/26/2020 at 7:47 PM
 * Author: Matt
 */

#include "mage/graphics/renderer.h"
#include <string>
#include <bmd/strutil.h>

void mage::Renderer::draw(mage::Mesh* mesh)
{
    mesh->render();
}

void mage::Renderer::drawPrimitive()
{
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0, 0.5f);
    glEnd();
}

#include <iostream>

void mage::Renderer::render(Shader& shader, Mesh& mesh)
{
    //if(mesh.getNumVertices() == 24) std::cout << "Rendering mesh " << mesh << " textures: " << mesh.getTextures().size() << std::endl;
    // Bind textures
    int diffuseTex = 1;
    int specularTex = 1;
    mesh.enable();
    if (!mesh.getTextures().empty())
    {
        for (int i = 0; i < mesh.getTextures().size(); ++i)
        {
            Texture tex = mesh.getTextures()[ i ];
            //std::cout << tex << std::endl;
            tex.enable(i);
            std::string number;
            if (tex.getType() == TEXTURE_DIFFUSE)
                number = std::to_string(diffuseTex++);
            if (tex.getType() == TEXTURE_SPECULAR);
            number = std::to_string(specularTex++);

            //char uniformName[128] = {"material."};
            char uniformName[128] = { "texture_" };
            concatStr(uniformName, tex.getType().c_str());
            concatStr(uniformName, number.c_str());
            shader.setUniform1i(uniformName, i);
        }

    }

    //int whichId;
    //glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &whichId);
    //std::cout << whichId << std::endl;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, pos));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, coords));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, normal));

    mesh.render();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    mesh.disable();
    if (!mesh.getTextures().empty())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void mage::Renderer::render(mage::Shader& shader, mage::Model& model)
{
    for (int i = 0; i < model.getMeshes().size(); i++)
    {
        render(shader, *model.getMeshes()[ i ]);
    }
}

// TODO: Should be pointers really
void mage::Renderer::render(mage::Shader& shader, mage::Mesh& mesh, const mage::Material& material)
{
    mesh.enable();
    Texture* diffuse;
    Texture* specular;
    Texture* emission;
    if (material.diffuse)
    {
        diffuse = material.diffuse;
        material.diffuse->enable(0);
    } else
    {
        diffuse = m_defaultDiffuse;
        diffuse->enable(0);
    }
    shader.setUniform1i("material.texture_diffuse", 0);

    if (material.specular)
    {
        specular = material.specular;
        material.specular->enable(1);
    } else
    {
        specular = m_defaultSpecular;
        specular->enable(1);
    }
    shader.setUniform1i("material.texture_specular", 1);

    if (material.emission)
    {
        emission = material.emission;
        material.emission->enable(2);
    } else
    {
        emission = m_defaultEmission;
        emission->enable(2);
    }
    shader.setUniform1i("material.texture_emission", 2);

    shader.setUniform1f("material.shininess", material.shininess);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(shader.getAttribLocation("position"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, pos));
    glVertexAttribPointer(shader.getAttribLocation("texCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, coords));
    glVertexAttribPointer(shader.getAttribLocation("normal"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, normal));
    glVertexAttribPointer(shader.getAttribLocation("color"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, color));

    mesh.render();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    mesh.disable();

    diffuse->disable(0);
    specular->disable(1);
    emission->disable(2);

}

void mage::Renderer::render(mage::Shader& shader, mage::Model& model, const mage::Material& material)
{
    for (int i = 0; i < model.getMeshes().size(); i++)
    {
        render(shader, *model.getMeshes()[ i ], material);
    }
}
