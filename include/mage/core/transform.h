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
 * File Name: transform.h
 * Date File Created: 8/14/2020 at 12:53 PM
 * Author: Matt
 */

#ifndef MAGE3D_TRANSFORM_H
#define MAGE3D_TRANSFORM_H


#include "mage3d_exported.h"
#include "mage/common.h"


namespace mage
{
	class Transform
	{
	public:
		mage3d_EXPORT explicit Transform(const vec3f& pos = vec3f(0), const mat4f& rot = mat4f(1), float scale = 1);
		mage3d_EXPORT virtual ~Transform() = default;

		mage3d_EXPORT mat4f getTransformation();

		mage3d_EXPORT bool hasChanged();
		mage3d_EXPORT void update();
		mage3d_EXPORT void rotate(float angle, const vec3f& axis);

        mage3d_EXPORT inline const vec3f& getPos() const { return m_pos; }
        mage3d_EXPORT inline vec3f& getPos() { return m_pos; }
        //mage3d_EXPORT inline const quaternion& getRotation() const { return m_rotation; }
        //mage3d_EXPORT inline quaternion& getRotation() { return m_rotation; }
        mage3d_EXPORT inline float getScale() const { return m_scale; }

        mage3d_EXPORT inline void setPos(const vec3f& pos) { m_pos = pos; }
        mage3d_EXPORT inline void setRotation(const mat4f& rot) { m_rotation = rot; }
        mage3d_EXPORT inline void setScale(float scale) { m_scale = scale; }
        mage3d_EXPORT inline void setParent(Transform* parent) { m_parent = parent; }

    protected:
	private:
		mat4f getParentMatrix() const;

		vec3f m_pos;
		mat4f m_rotation;
		float m_scale;

		Transform* m_parent;
		mutable mat4f m_parentMatrix;
		mutable vec3f m_oldPos{};
		mutable mat4f m_oldRotation{};
		mutable float m_oldScale{};
		mutable bool m_oldStuffInitialized;
	};

}


#endif //MAGE3D_TRANSFORM_H
