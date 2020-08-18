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
 * File Name: vertex.h
 * Date File Created: 7/27/2020 at 12:43 AM
 * Author: Matt
 */

#ifndef MAGE3D_VERTEX_H
#define MAGE3D_VERTEX_H

#include "mage3d_exported.h"
#include <bmd/types.h>
#include <glm/glm.hpp>
#include <ostream>

namespace mage
{

    struct Position
    {
        float x = 0;
        float y = 0;
        float z = 0;

        Position(float _x, float _y, float _z) :
                x(_x),
                y(_y),
                z(_z) { }

        friend std::ostream& operator<<(std::ostream& os, const Position& position)
        {
            os << "x: " << position.x << " y: " << position.y << " z: " << position.z;
            return os;
        }
    };


    struct TexCoords
    {
        float u = 0;
        float v = 0;

        TexCoords(float _u, float _v) :
                u(_u),
                v(_v) { }

        friend std::ostream& operator<<(std::ostream& os, const TexCoords& coords)
        {
            os << "u: " << coords.u << " v: " << coords.v;
            return os;
        }
    };

    struct Normal
    {
        float x = 0;
        float y = 0;
        float z = 0;

        Normal(float _x, float _y, float _z) :
                x(_x),
                y(_y),
                z(_z) { }

        friend std::ostream& operator<<(std::ostream& os, const Normal& normal)
        {
            os << "x: " << normal.x << " y: " << normal.y << " z: " << normal.z;
            return os;
        }
    };


    struct Vertex
    {
        Position pos;
        Normal normal;
        TexCoords coords;

        explicit Vertex(Position _pos, Normal _normal = Normal(0, 0, 0),
                        TexCoords _texCoords = TexCoords(0, 0)) :
                pos(_pos),
                normal(_normal),
                coords(_texCoords) { }

        void setPos(float x, float y, float z)
        {
            pos.x = x;
            pos.y = y;
            pos.z = z;
        }

        void setTexCoords(float u, float v)
        {
            coords.u = u;
            coords.v = v;
        }

        void setNormalCoords(float x, float y, float z)
        {
            normal.x = x;
            normal.y = y;
            normal.z = z;
        }

        vec3f posVec() { return vec3f(pos.x, pos.y, pos.z); }

        vec2f texCoordsVec() { return vec2f(coords.u, coords.v); }

        vec3f normalVec() { return vec3f(normal.x, normal.y, normal.z); }


        friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
        {
            os << "pos: " << vertex.pos << " normal: " << vertex.normal << " coords: " << vertex.coords;
            return os;
        }
    };
}


#endif //MAGE3D_VERTEX_H
