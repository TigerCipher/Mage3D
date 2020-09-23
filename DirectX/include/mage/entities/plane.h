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
 * File Name: plane.h
 * Date File Created: 9/21/2020 at 11:12 PM
 * Author: Matt
 */

#ifndef MAGE3DX_PLANE_H
#define MAGE3DX_PLANE_H


//#include "pch.h"
#include "indexedtrianglelist.h"
#include "mage/core/mathhelper.h"


namespace mage
{
    class Plane
    {
    public:
        template<class V>
        static IndexedTriangleList<V> makeTesselated(int divisionsX, int divisionsY)
        {
            assert(divisionsX >= 1);
            assert(divisionsY >= 1);

            constexpr float width = 2.0f;
            constexpr float height = 2.0f;
            const int numVertsX = divisionsX + 1;
            const int numVertsY = divisionsY + 1;
            list<V> vertices(numVertsX * numVertsY);

            {
                const float sideX = width / 2.0f;
                const float sideY = height / 2.0f;
                const float divisionSizeX = width / float(divisionsX);
                const float divisionSizeY = height / float(divisionsY);
                const auto bottomLeft = dx::XMVectorSet(-sideX, -sideY, 0.0f, 0.0f);

                for (int y = 0, i = 0; y < numVertsY; y++)
                {
                    const float yPos = float(y) * divisionSizeY;
                    for (int x = 0; x < numVertsX; x++, i++)
                    {
                        const auto v = dx::XMVectorAdd(
                                bottomLeft,
                                dx::XMVectorSet(float(x) * divisionSizeX, yPos, 0.0f, 0.0f)
                                                      );
                        dx::XMStoreFloat3(&vertices[ i ].pos, v);
                    }
                }
            }

            list<ushort> indices;
            indices.reserve(pow2(divisionsX * divisionsY) * 6);
            {
                const auto vxy2i = [ numVertsX ](size_t x, size_t y)
                {
                    return (ushort) (y * numVertsX + x);
                };
                for (size_t y = 0; y < divisionsY; y++)
                {
                    for (size_t x = 0; x < divisionsX; x++)
                    {
                        const std::array<ushort, 4> indexArray =
                                { vxy2i(x, y), vxy2i(x + 1, y), vxy2i(x, y + 1), vxy2i(x + 1, y + 1) };
                        indices.push_back(indexArray[ 0 ]);
                        indices.push_back(indexArray[ 2 ]);
                        indices.push_back(indexArray[ 1 ]);
                        indices.push_back(indexArray[ 1 ]);
                        indices.push_back(indexArray[ 2 ]);
                        indices.push_back(indexArray[ 3 ]);
                    }
                }
            }

            return { std::move(vertices), std::move(indices) };
        }

        template<class V>
        static IndexedTriangleList<V> make()
        {
            return makeTesselated<V>(1, 1);
        }
    };
}

#endif //MAGE3DX_PLANE_H
