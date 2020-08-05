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
	mesh.enable();
	if (!mesh.getTextures().empty())
	{
		for (int i = 0; i < mesh.getTextures().size(); ++i)
		{
			Texture tex = mesh.getTextures()[i];
			//std::cout << tex << std::endl;
			tex.enable(i);
			std::string number;
			if(tex.getType() == TEXTURE_DIFFUSE)
				number = std::to_string(diffuseTex++);

			//char uniformName[128] = {"material."};
			char uniformName[128] = {"texture_"};
			concatStr(uniformName, tex.getType().c_str());
			concatStr(uniformName, number.c_str());
			shader.setUniform1i(uniformName, i);
		}

	}

	//int whichId;
	//glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &whichId);
	//std::cout << whichId << std::endl;
	for (int i = 0; i < shader.getNumAttribs(); i++)
	{
		glEnableVertexAttribArray(i);
		AttribInfo info;
		if (shader.getAttribInfo(i, &info))
		{
			if (strcmp(info.name, "position") == 0)
				glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
									  (void*) offsetof(Vertex, pos));
			else if (strcmp(info.name, "color") == 0)
				glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
									  (void*) offsetof(Vertex, color));
			else if (strcmp(info.name, "texCoord") == 0)
				glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
									  (void*) offsetof(Vertex, coords));
			else if (strcmp(info.name, "normal") == 0)
				glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
									  (void*) offsetof(Vertex, normal));
			// TODO: Handle unknown/user defined attribs
		}
	}

	mesh.render();
	for (int i = 0; i < shader.getNumAttribs(); i++)
		glDisableVertexAttribArray(i);
	mesh.disable();
	//if (!mesh.getTextures().empty())
	//{
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
}

void mage::Renderer::render(mage::Shader& shader, mage::Model& model)
{
	for(int i = 0; i < model.getMeshes().size(); i++)
	{
		render(shader, *model.getMeshes()[i]);
	}
}
