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
 * File Name: cone.h
 * Date File Created: 9/21/2020 at 11:06 PM
 * Author: Matt
 */

#ifndef MAGE3DX_CONE_H
#define MAGE3DX_CONE_H


//#include "pch.h"
#include "mage/core/mathhelper.h"
#include "indexedtrianglelist.h"


namespace mage
{
    class Cone
    {
    public:
        template<class V>
        static IndexedTriangleList<V> makeTesselated(int longDiv)
        {
            assert(longDiv >= 3);

            const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
            const float longitudeAngle = 2.0f * PI / (float)longDiv;

            // base vertices
            list<V> vertices;
            for (int iLong = 0; iLong < longDiv; iLong++)
            {
                vertices.emplace_back();
                auto v = dx::XMVector3Transform(
                        base,
                        dx::XMMatrixRotationZ(longitudeAngle * (float)iLong)
                                               );
                dx::XMStoreFloat3(&vertices.back().pos, v);
            }
            // the center
            vertices.emplace_back();
            vertices.back().pos = { 0.0f, 0.0f, -1.0f };
            const auto iCenter = (ushort) (vertices.size() - 1);
            // the tip :darkness:
            vertices.emplace_back();
            vertices.back().pos = { 0.0f, 0.0f, 1.0f };
            const auto iTip = (ushort) (vertices.size() - 1);


            // base indices
            std::vector<ushort> indices;
            for (ushort iLong = 0; iLong < longDiv; iLong++)
            {
                indices.push_back(iCenter);
                indices.push_back((iLong + 1) % longDiv);
                indices.push_back(iLong);
            }

            // cone indices
            for (ushort iLong = 0; iLong < longDiv; iLong++)
            {
                indices.push_back(iLong);
                indices.push_back((iLong + 1) % longDiv);
                indices.push_back(iTip);
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

#endif //MAGE3DX_CONE_H
