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
 * File Name: gameobject.h
 * Date File Created: 8/14/2020 at 9:17 PM
 * Author: Matt
 */

#ifndef MAGE3D_GAMEOBJECT_H
#define MAGE3D_GAMEOBJECT_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "display.h"
#include "transform.h"
#include "input.h"
#include "mage/graphics/shader.h"
#include "mage/graphics/renderengine.h"
#include "engine.h"

namespace mage
{
    class GameComponent;

    class GameObject
    {
    public:
        mage3d_EXPORT GameObject() = default;
        mage3d_EXPORT virtual ~GameObject();

        mage3d_EXPORT GameObject* addChild(GameObject* child);
        mage3d_EXPORT GameObject* addComponent(GameComponent* component);

        mage3d_EXPORT void inputAll(Input* _input, float delta);
        mage3d_EXPORT void updateAll(float delta);
        mage3d_EXPORT void renderAll(const RenderEngine* renderEngine);

        mage3d_EXPORT list<GameObject*> getAttachedChildren();

        inline Transform& getTransform() { return m_transform; }
    protected:
    private:
        void input(Input* _input, float delta);
        void update(float delta);
        void render(const RenderEngine* renderEngine);

        list<GameObject*> m_children;
        list<GameComponent*> m_components;
        Transform m_transform;

    };

}


#endif //MAGE3D_GAMEOBJECT_H
