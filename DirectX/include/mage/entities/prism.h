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
 * File Name: prism.h
 * Date File Created: 9/21/2020 at 11:16 PM
 * Author: Matt
 */

#ifndef MAGE3DX_PRISM_H
#define MAGE3DX_PRISM_H


#include "pch.h"
#include "indexedtrianglelist.h"
#include "mage/core/mathhelper.h"

namespace mage
{
    class Prism
    {
    public:
        template<class V>
        static IndexedTriangleList<V> makeTesselated(int longDiv)
        {
            assert(longDiv >= 3);

            const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
            const auto offset = dx::XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
            const float longitudeAngle = 2.0f * PI / (float) longDiv;

            // near center
            list<V> vertices;
            vertices.emplace_back();
            vertices.back().pos = { 0.0f, 0.0f, -1.0f };
            const auto iCenterNear = (ushort) (vertices.size() - 1);
            // far center
            vertices.emplace_back();
            vertices.back().pos = { 0.0f, 0.0f, 1.0f };
            const auto iCenterFar = (ushort) (vertices.size() - 1);

            // base vertices
            for (int iLong = 0; iLong < longDiv; iLong++)
            {
                // near base
                {
                    vertices.emplace_back();
                    auto v = dx::XMVector3Transform(
                            base,
                            dx::XMMatrixRotationZ(longitudeAngle * (float) iLong)
                                                   );
                    dx::XMStoreFloat3(&vertices.back().pos, v);
                }
                // far base
                {
                    vertices.emplace_back();
                    auto v = dx::XMVector3Transform(
                            base,
                            dx::XMMatrixRotationZ(longitudeAngle * (float) iLong)
                                                   );
                    v = dx::XMVectorAdd(v, offset);
                    dx::XMStoreFloat3(&vertices.back().pos, v);
                }
            }

            // side indices
            list<ushort> indices;
            for (ushort iLong = 0; iLong < longDiv; iLong++)
            {
                const auto i = iLong * 2;
                const auto mod = longDiv * 2;
                indices.push_back(i + 2);
                indices.push_back((i + 2) % mod + 2);
                indices.push_back(i + 1 + 2);
                indices.push_back((i + 2) % mod + 2);
                indices.push_back((i + 3) % mod + 2);
                indices.push_back(i + 1 + 2);
            }

            // base indices
            for (ushort iLong = 0; iLong < longDiv; iLong++)
            {
                const auto i = iLong * 2;
                const auto mod = longDiv * 2;
                indices.push_back(i + 2);
                indices.push_back(iCenterNear);
                indices.push_back((i + 2) % mod + 2);
                indices.push_back(iCenterFar);
                indices.push_back(i + 1 + 2);
                indices.push_back((i + 3) % mod + 2);
            }

            return { std::move(vertices), std::move(indices) };
        }

        template<class V>
        static IndexedTriangleList<V> make()
        {
            return makeTesselated<V>(24);
        }
    };
}

#endif //MAGE3DX_PRISM_H
