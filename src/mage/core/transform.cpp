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
 * File Name: transform.cpp
 * Date File Created: 8/14/2020 at 12:53 PM
 * Author: Matt
 */

#include "mage/core/transform.h"
#include <glm/gtx/quaternion.hpp>

mage::Transform::Transform(const vec3f& pos, const mat4f& rot, float scale) :
        m_pos(pos),
        m_rotation(rot),
        m_scale(scale),
        m_parentMatrix(1),
        m_oldStuffInitialized(false)
{
    m_parent = nullptr;
}

bool mage::Transform::hasChanged()
{
    return (m_parent && m_parent->hasChanged()) || (m_pos != m_oldPos) || (m_rotation != m_oldRotation) ||
           (m_scale != m_oldScale);
}

void mage::Transform::update()
{
    if(m_oldStuffInitialized)
    {
        m_oldPos = m_pos;
        m_oldRotation = m_rotation;
        m_oldScale = m_scale;
    }else
    {
        m_oldPos = m_pos + vec3f(1);
        m_oldRotation = m_rotation * 0.5f;
        m_oldScale = m_scale + 1;
        m_oldStuffInitialized = true;
    }
}

void mage::Transform::rotate(float angle, const vec3f& axis)
{
    m_rotation = glm::rotate(m_rotation, angle, axis);
}

mat4f mage::Transform::getParentMatrix() const
{
    if(m_parent && m_parent->hasChanged())
        m_parentMatrix = m_parent->getTransformation();
    return m_parentMatrix;
}

mat4f mage::Transform::getTransformation()
{
    mat4f transformation = glm::translate(mat4f(1), m_pos);
    mat4f rotMat = m_rotation;
    transformation *= rotMat;
    transformation = glm::scale(transformation, vec3f(m_scale));
    return getParentMatrix() * transformation;
}
