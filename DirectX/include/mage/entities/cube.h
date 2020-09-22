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
 * File Name: cube.h
 * Date File Created: 9/21/2020 at 11:10 PM
 * Author: Matt
 */

#ifndef MAGE3DX_CUBE_H
#define MAGE3DX_CUBE_H


#include "pch.h"
#include "indexedtrianglelist.h"
#include "mage/core/mathhelper.h"

namespace mage
{
    class Cube
    {
    public:
        template<class V>
        static IndexedTriangleList<V> make()
        {

            constexpr float side = 1.0f / 2.0f;

            list<vec3f> vertices;
            vertices.emplace_back(-side, -side, -side);
            vertices.emplace_back(side, -side, -side);
            vertices.emplace_back(-side, side, -side);
            vertices.emplace_back(side, side, -side);
            vertices.emplace_back(-side, -side, side);
            vertices.emplace_back(side, -side, side);
            vertices.emplace_back(-side, side, side);
            vertices.emplace_back(side, side, side);

            list<V> verts(vertices.size());
            for (size_t i = 0; i < vertices.size(); i++)
            {
                verts[ i ].pos = vertices[ i ];
            }
            return {
                    std::move(verts), {
                            0, 2, 1, 2, 3, 1,
                            1, 3, 5, 3, 7, 5,
                            2, 6, 3, 3, 6, 7,
                            4, 5, 7, 4, 7, 6,
                            0, 4, 2, 2, 4, 6,
                            0, 1, 4, 1, 5, 4
                    }
            };
        }
    };
}

#endif //MAGE3DX_CUBE_H
