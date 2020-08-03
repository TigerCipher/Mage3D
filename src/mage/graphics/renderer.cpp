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

void mage::Renderer::render(Shader& shader, Mesh& mesh)
{
	mesh.enable();
	for(int i = 0; i < shader.getNumAttribs(); i++)
	{
		glEnableVertexAttribArray(i);
		AttribInfo info;
		if(shader.getAttribInfo(i, &info))
		{
			if(strcmp(info.name, "position") == 0)
				glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));
			else if(strcmp(info.name, "color") == 0)
				glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));
			else if(strcmp(info.name, "texCoord") == 0)
				glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, coords));
			else if(strcmp(info.name, "normal") == 0)
				glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
			// TODO: Handle unknown/user defined attribs
		}
	}
	mesh.render();
	for(int i = 0; i < shader.getNumAttribs(); i++)
		glDisableVertexAttribArray(i);
	mesh.disable();
}
