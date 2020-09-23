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
 * File Name: sphere.h
 * Date File Created: 9/21/2020 at 11:18 PM
 * Author: Matt
 */

#ifndef MAGE3DX_SPHERE_H
#define MAGE3DX_SPHERE_H


//#include "pch.h"
#include "indexedtrianglelist.h"
#include "mage/core/mathhelper.h"

namespace mage
{
    class Sphere
    {
    public:
        template<class V>
        static IndexedTriangleList<V> makeTesselated(int latDiv, int longDiv)
        {
            assert(latDiv >= 3);
            assert(longDiv >= 3);

            constexpr float radius = 1.0f;
            const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
            const float lattitudeAngle = PI / (float) latDiv;
            const float longitudeAngle = 2.0f * PI / (float) longDiv;

            list<V> vertices;
            for (int iLat = 1; iLat < latDiv; iLat++)
            {
                const auto latBase = dx::XMVector3Transform(
                        base,
                        dx::XMMatrixRotationX(lattitudeAngle * (float) iLat)
                                                           );
                for (int iLong = 0; iLong < longDiv; iLong++)
                {
                    vertices.emplace_back();
                    auto v = dx::XMVector3Transform(
                            latBase,
                            dx::XMMatrixRotationZ(longitudeAngle * (float) iLong)
                                                   );
                    dx::XMStoreFloat3(&vertices.back().pos, v);
                }
            }

            // add the cap vertices
            const auto iNorthPole = (ushort) vertices.size();
            vertices.emplace_back();
            dx::XMStoreFloat3(&vertices.back().pos, base);
            const auto iSouthPole = (ushort) vertices.size();
            vertices.emplace_back();
            dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

            const auto calcIdx = [ latDiv, longDiv ](ushort iLat, ushort iLong)
            {
                return iLat * longDiv + iLong;
            };
            list<ushort> indices;
            for (ushort iLat = 0; iLat < latDiv - 2; iLat++)
            {
                for (ushort iLong = 0; iLong < longDiv - 1; iLong++)
                {
                    indices.push_back(calcIdx(iLat, iLong));
                    indices.push_back(calcIdx(iLat + 1, iLong));
                    indices.push_back(calcIdx(iLat, iLong + 1));
                    indices.push_back(calcIdx(iLat, iLong + 1));
                    indices.push_back(calcIdx(iLat + 1, iLong));
                    indices.push_back(calcIdx(iLat + 1, iLong + 1));
                }
                // wrap band
                indices.push_back(calcIdx(iLat, longDiv - 1));
                indices.push_back(calcIdx(iLat + 1, longDiv - 1));
                indices.push_back(calcIdx(iLat, 0));
                indices.push_back(calcIdx(iLat, 0));
                indices.push_back(calcIdx(iLat + 1, longDiv - 1));
                indices.push_back(calcIdx(iLat + 1, 0));
            }

            // cap fans
            for (ushort iLong = 0; iLong < longDiv - 1; iLong++)
            {
                // north
                indices.push_back(iNorthPole);
                indices.push_back(calcIdx(0, iLong));
                indices.push_back(calcIdx(0, iLong + 1));
                // south
                indices.push_back(calcIdx(latDiv - 2, iLong + 1));
                indices.push_back(calcIdx(latDiv - 2, iLong));
                indices.push_back(iSouthPole);
            }
            // wrap triangles
            // north
            indices.push_back(iNorthPole);
            indices.push_back(calcIdx(0, longDiv - 1));
            indices.push_back(calcIdx(0, 0));
            // south
            indices.push_back(calcIdx(latDiv - 2, 0));
            indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
            indices.push_back(iSouthPole);

            return { std::move(vertices), std::move(indices) };
        }

        template<class V>
        static IndexedTriangleList<V> make()
        {
            return makeTesselated<V>(12, 24);
        }
    };
}

#endif //MAGE3DX_SPHERE_H
