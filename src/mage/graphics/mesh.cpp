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

#include <utility>


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
    // TODO Perhaps only create the buffers in a separate 'init' function
    // So that models and other mesh using resources can be loaded before the display is shown?
    // Same with things like textures
    // That or have the engine render a splashscreen while things load?
    // Though if we wanted a traditional animated splashscreen or anything, might have to introduce multi threading
    int er;
    checkGlError(er);
    glGenVertexArrays(1, &m_vaoId);
    glBindVertexArray(m_vaoId);
    createBuffers(vertices.data(), indices.data());
    checkGlError(er);
    disable();
}

mage::Mesh::Mesh(list<mage::Vertex>& vertices, list<uint>& indices, const list<mage::Texture>& textures) :
        m_textures(textures),
        m_vertices(vertices),
        m_indices(indices)
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
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
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

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, coords));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*) offsetof(Vertex, normal));
}

void mage::Mesh::render() const
{
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
}

void mage::Mesh::enable() const
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

std::ostream& mage::operator<<(std::ostream& os, const mage::Mesh& mesh)
{
    os << "m_vaoId: " << mesh.m_vaoId << " m_vbo: " << mesh.m_vbo << " m_ebo: " << mesh.m_ebo
       << " m_numVertices: " << mesh.m_numVertices << " m_numIndices: " << mesh.m_numIndices;
    return os;
}

mage::Mesh::Mesh(const list<float>& vertices, mage::MeshType type)
{
    glGenVertexArrays(1, &m_vaoId);
    glBindVertexArray(m_vaoId);
    m_numVertices = vertices.size();
    switch (type)
    {
        case QUAD:
            m_numAttribsPerVert = 4;
            createBuffersQuad(vertices);
            break;
        case CUBE:
            m_numAttribsPerVert = 3;
            createBuffersCube(vertices);
            break;
        case STANDARD:
            LOG_CRITICAL("This particular mesh constructor does not currently support the \"standard\" type");
            assert(false);
            break;
        default:
            LOG_CRITICAL("Unknown mesh type");
            assert(false);
    }
    disable_basic();
}

void mage::Mesh::createBuffersQuad(list<float> vertices)
{
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void*) (2 * sizeof(float)));
}

void mage::Mesh::enable_basic()
{
    glBindVertexArray(m_vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void mage::Mesh::disable_basic()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void mage::Mesh::draw_basic()
{
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices / m_numAttribsPerVert);
}

void mage::Mesh::createBuffersCube(list<float> vertices)
{
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
}





