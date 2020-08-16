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
 * File Name: gamecomponent.h
 * Date File Created: 8/14/2020 at 9:13 PM
 * Author: Matt
 */

#ifndef MAGE3D_GAMECOMPONENT_H
#define MAGE3D_GAMECOMPONENT_H

#include "mage3d_exported.h"
#include "mage/common.h"
#include "mage/graphics/renderengine.h"
#include "gameobject.h"


namespace mage
{
    class Shader;
    class Renderer;

    class GameComponent
    {
    public:
        mage3d_EXPORT virtual ~GameComponent() = default;
        mage3d_EXPORT virtual void input(Input* _input, float delta) {}
        mage3d_EXPORT virtual void update(float delta) {}
        mage3d_EXPORT virtual void render(const RenderEngine* renderEngine) {}
        mage3d_EXPORT inline void setParent(GameObject* parent) { m_parent = parent; }
        mage3d_EXPORT inline Transform& getTransform() { return m_parent->getTransform(); }
        [[nodiscard]] mage3d_EXPORT inline const Transform& getTransform() const { return m_parent->getTransform(); }
    private:
        GameObject* m_parent{};
    };
}

#endif //MAGE3D_GAMECOMPONENT_H
