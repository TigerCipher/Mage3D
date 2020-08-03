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


mage::Mesh::Mesh(mage::Vertex vertices[], uint verticesSize, uint indices[], uint indicesSize) :
		m_numVertices(verticesSize),
		m_numIndices(indicesSize)
{
	assert(m_numIndices % 3 == 0);
	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);
	createBuffers(vertices, indices);
	disable();
}

mage::Mesh::Mesh(list<Vertex> vertices, list<uint> indices)
{
	m_numVertices = vertices.size();
	m_numIndices = indices.size();
	assert(m_numIndices % 3 == 0);
	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);
	createBuffers(vertices.data(), indices.data());
	disable();
}

mage::Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vaoId);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void mage::Mesh::createBuffers(Vertex vertices[], uint indices[])
{
	// Set up vertex buffer object
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	// Set up element buffer object
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(uint), indices, GL_STATIC_DRAW);
}

void mage::Mesh::enableAttribPointers()
{
	// Currently this requires attrib location 0 to be a vec3 and 1 to be a vec4
	// Surely there is a way to generalize this?
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	// vec3 pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));
	// vec4 color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));
	// vec2 tex coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, coords));
	// vec3 normal coords
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

	// TODO: Perhaps make Mesh a general class, with derived classes for more simple meshes and more advanced?
	// Not every mesh would need a color or texture or etc. Would also allow the use of shaders that dont require the same input attribs as every other
	// In fact, the attrib handling is dealt with during rendering, maybe have renderer class take care of it?
	// Mesh could have some sort of type flag (i.e SIMPLE, NO_TEXTURE, etc etc)
	// Or maybe it should depend on the shader being used?
	// i.e could be renderer.render(shader, mesh)
	// Shader could parse through the glsl code and find the number of "in" types for the vertex shader?
	// Could also get their name, i.e if the "in" is called 'normal' or 'normalCoords' then we know to use Vertex.normal attrib
}

void mage::Mesh::disableAttribPointers()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void mage::Mesh::render()
{
	enable();
	enableAttribPointers();
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	disableAttribPointers();
	disable();
}

void mage::Mesh::enable()
{
	glBindVertexArray(m_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void mage::Mesh::disable()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



