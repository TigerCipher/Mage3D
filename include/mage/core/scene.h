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
 * File Name: scene.h
 * Date File Created: 8/16/2020 at 9:37 PM
 * Author: Matt
 */

#ifndef MAGE3D_SCENE_H
#define MAGE3D_SCENE_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "engine.h"
#include "input.h"
#include "mage/graphics/renderengine.h"

namespace mage
{
    class GameObject;
    class Scene
    {
    public:
        mage3d_EXPORT Scene() = default;
        mage3d_EXPORT virtual ~Scene() = default;
        mage3d_EXPORT virtual void preInit();
        mage3d_EXPORT virtual void init() = 0;
        mage3d_EXPORT virtual void addToScene(GameObject* child);
        mage3d_EXPORT virtual void input(Input* pInput, float delta);
        mage3d_EXPORT virtual void update(float delta);
        mage3d_EXPORT virtual void render(const RenderEngine* renderEngine);

    protected:
    private:
        SharedPtr<GameObject> m_root{};
    };

}


#endif //MAGE3D_SCENE_H
