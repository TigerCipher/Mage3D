/*
 * Mage3DX
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
 * File Name: indexedtrianglelist.h
 * Date File Created: 9/21/2020 at 10:53 PM
 * Author: Matt
 */

#ifndef MAGE3DX_INDEXEDTRIANGLELIST_H
#define MAGE3DX_INDEXEDTRIANGLELIST_H


#include <utility>

#include "pch.h"
#include "mage/core/mathhelper.h"

namespace mage
{

    template<class T>
    class IndexedTriangleList
    {
    public:
        IndexedTriangleList() = default;

        IndexedTriangleList(list<T> verts, list<ushort> ints) :
                vertices(std::move(verts)),
                indices(std::move(ints))
        {
            assert(vertices.size() > 2);
            assert(indices.size() % 3 == 0);
        }

        void transform(mat4f matrix)
        {
            for(auto& v : vertices)
            {
                const vec4f pos = dx::XMLoadFloat3(&v.pos);
                dx::XMStoreFloat3(&v.pos, dx::XMVector3Transform(pos, matrix));
            }
        }

        list<T> vertices;
        list<ushort> indices;
    };

}

#endif //MAGE3DX_INDEXEDTRIANGLELIST_H
