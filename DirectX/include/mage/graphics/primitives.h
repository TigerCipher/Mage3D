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
 * File Name: primitives.h
 * Date File Created: 9/24/2020 at 12:08 AM
 * Author: Matt
 */

#ifndef MAGE3DX_PRIMITIVES_H
#define MAGE3DX_PRIMITIVES_H

#include "mage/core/mathhelper.h"

namespace mage
{
    template<class T>
    class IndexedTriangleList
    {
    public:
        IndexedTriangleList() = default;

        IndexedTriangleList(list<T> verts, list<ushort> ints) : vertices(std::move(verts)), indices(std::move(ints))
        {
            assert(vertices.size() > 2);
            assert(indices.size() % 3 == 0);
        }

        void transform(mat4f matrix)
        {
            for (auto& v : vertices)
            {
                const vec4f pos = dx::XMLoadFloat3(&v.pos);
                dx::XMStoreFloat3(&v.pos, dx::XMVector3Transform(pos, matrix));
            }
        }

        list<T> vertices;
        list<ushort> indices;
    };

    class Cone
    {
    public:
        template<class V>
        static IndexedTriangleList<V> makeTesselated(int longDiv)
        {
            assert(longDiv >= 3);

            const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
            const float longitudeAngle = 2.0f * PI / (float) longDiv;

            // base vertices
            list<V> vertices;
            for (int iLong = 0; iLong < longDiv; iLong++)
            {
                vertices.emplace_back();
                auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * (float) iLong));
                dx::XMStoreFloat3(&vertices.back().pos, v);
            }
            // the center
            vertices.emplace_back();
            vertices.back().pos = { 0.0f, 0.0f, -1.0f };
            const auto iCenter = (ushort)(vertices.size() - 1);
            // the tip :darkness:
            vertices.emplace_back();
            vertices.back().pos = { 0.0f, 0.0f, 1.0f };
            const auto iTip = (ushort)(vertices.size() - 1);


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
            for (size_t i = 0; i < vertices.size(); i++) { verts[ i ].pos = vertices[ i ]; }
            return { std::move(verts), { 0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5, 2, 6, 3, 3, 6, 7,
                                         4, 5, 7, 4, 7, 6, 0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4 } };
        }

        template<class V>
        static IndexedTriangleList<V> makeSkinned()
        {
            constexpr float side = 1.0f / 2.0f;

            list<V> vertices(14);

            vertices[ 0 ].pos = { -side, -side, -side };
            vertices[ 0 ].tex = { 2.0f / 3.0f, 0.0f / 4.0f };
            vertices[ 1 ].pos = { side, -side, -side };
            vertices[ 1 ].tex = { 1.0f / 3.0f, 0.0f / 4.0f };
            vertices[ 2 ].pos = { -side, side, -side };
            vertices[ 2 ].tex = { 2.0f / 3.0f, 1.0f / 4.0f };
            vertices[ 3 ].pos = { side, side, -side };
            vertices[ 3 ].tex = { 1.0f / 3.0f, 1.0f / 4.0f };
            vertices[ 4 ].pos = { -side, -side, side };
            vertices[ 4 ].tex = { 2.0f / 3.0f, 3.0f / 4.0f };
            vertices[ 5 ].pos = { side, -side, side };
            vertices[ 5 ].tex = { 1.0f / 3.0f, 3.0f / 4.0f };
            vertices[ 6 ].pos = { -side, side, side };
            vertices[ 6 ].tex = { 2.0f / 3.0f, 2.0f / 4.0f };
            vertices[ 7 ].pos = { side, side, side };
            vertices[ 7 ].tex = { 1.0f / 3.0f, 2.0f / 4.0f };
            vertices[ 8 ].pos = { -side, -side, -side };
            vertices[ 8 ].tex = { 2.0f / 3.0f, 4.0f / 4.0f };
            vertices[ 9 ].pos = { side, -side, -side };
            vertices[ 9 ].tex = { 1.0f / 3.0f, 4.0f / 4.0f };
            vertices[ 10 ].pos = { -side, -side, -side };
            vertices[ 10 ].tex = { 3.0f / 3.0f, 1.0f / 4.0f };
            vertices[ 11 ].pos = { -side, -side, side };
            vertices[ 11 ].tex = { 3.0f / 3.0f, 2.0f / 4.0f };
            vertices[ 12 ].pos = { side, -side, -side };
            vertices[ 12 ].tex = { 0.0f / 3.0f, 1.0f / 4.0f };
            vertices[ 13 ].pos = { side, -side, side };
            vertices[ 13 ].tex = { 0.0f / 3.0f, 2.0f / 4.0f };

            return { std::move(vertices),
                     {
                             0,  2,  1,  2,  3,  1,//
                             4,  8,  5,  5,  8,  9,//
                             2,  6,  3,  3,  6,  7,//
                             4,  5,  7,  4,  7,  6,//
                             2,  10, 11, 2,  11, 6,//
                             12, 3,  7,  12, 7,  13//
                     } };
        }
    };

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
                        const auto v = dx::XMVectorAdd(bottomLeft,
                                                       dx::XMVectorSet(float(x) * divisionSizeX, yPos, 0.0f, 0.0f));
                        dx::XMStoreFloat3(&vertices[ i ].pos, v);
                    }
                }
            }

            list<ushort> indices;
            indices.reserve(pow2(divisionsX * divisionsY) * 6);
            {
                const auto vxy2i = [ numVertsX ](size_t x, size_t y) { return (ushort)(y * numVertsX + x); };
                for (size_t y = 0; y < divisionsY; y++)
                {
                    for (size_t x = 0; x < divisionsX; x++)
                    {
                        const std::array<ushort, 4> indexArray = { vxy2i(x, y), vxy2i(x + 1, y), vxy2i(x, y + 1),
                                                                   vxy2i(x + 1, y + 1) };
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
            const auto iCenterNear = (ushort)(vertices.size() - 1);
            // far center
            vertices.emplace_back();
            vertices.back().pos = { 0.0f, 0.0f, 1.0f };
            const auto iCenterFar = (ushort)(vertices.size() - 1);

            // base vertices
            for (int iLong = 0; iLong < longDiv; iLong++)
            {
                // near base
                {
                    vertices.emplace_back();
                    auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * (float) iLong));
                    dx::XMStoreFloat3(&vertices.back().pos, v);
                }
                // far base
                {
                    vertices.emplace_back();
                    auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * (float) iLong));
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
                const auto latBase = dx::XMVector3Transform(base, dx::XMMatrixRotationX(lattitudeAngle * (float) iLat));
                for (int iLong = 0; iLong < longDiv; iLong++)
                {
                    vertices.emplace_back();
                    auto v = dx::XMVector3Transform(latBase, dx::XMMatrixRotationZ(longitudeAngle * (float) iLong));
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

            const auto calcIdx = [ latDiv, longDiv ](ushort iLat, ushort iLong) { return iLat * longDiv + iLong; };
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

}// namespace mage

#endif//MAGE3DX_PRIMITIVES_H
