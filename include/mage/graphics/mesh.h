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
 * File Name: mesh.h
 * Date File Created: 7/26/2020 at 7:19 PM
 * Author: Matt
 */

#ifndef MAGE3D_MESH_H
#define MAGE3D_MESH_H
#include <GL/glew.h>
#include "mage3d_exported.h"
#include "mage/core/platform.h"
#include "vertex.h"

namespace mage
{
	class Mesh
	{
	public:
		mage3d_EXPORT Mesh(Vertex vertices[], uint verticesSize, uint indices[], uint indicesSize);
		mage3d_EXPORT ~Mesh();

		mage3d_EXPORT void render();


	protected:
	private:

		void createBuffers(Vertex* data, uint* indices);
		void enableAttribPointers();
		void disableAttribPointers();
		void enable();
		void disable();

		GLuint m_vaoId;
		GLuint m_vbo;
		GLuint m_ebo;

		uint m_numVertices;
		uint m_numIndices;
	};

}
#endif //MAGE3D_MESH_H
