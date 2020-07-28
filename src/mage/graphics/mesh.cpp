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
 * File Name: mesh.cpp
 * Date File Created: 7/26/2020 at 7:19 PM
 * Author: Matt
 */


#include "mage/graphics/mesh.h"
#include "mage/core/stdcolor.h"

#define NUM_COORDS 3

mage::Mesh::Mesh(Vertex data[])
{
	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);
	storeDataInAttribList(data);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

mage::Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vaoId);
	glDeleteBuffers(1, &m_vbo);
}

void mage::Mesh::storeDataInAttribList(Vertex data[])
{
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(Vertex), data, GL_STATIC_DRAW);
}


void mage::Mesh::render()
{
	glBindVertexArray(m_vaoId);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glBindVertexArray(0);
}

