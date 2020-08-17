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
 * File Name: scene.cpp
 * Date File Created: 8/16/2020 at 9:37 PM
 * Author: Matt
 */

#include "mage/core/scene.h"
#include "mage/core/gameobject.h"

void mage::Scene::addToScene(mage::GameObject* child)
{
    m_root->addChild(child);
}

void mage::Scene::input(mage::Input* pInput, float delta)
{
    m_root->inputAll(pInput, delta);
}

void mage::Scene::update(float delta)
{
    m_root->updateAll(delta);
}

void mage::Scene::render(const mage::RenderEngine* renderEngine)
{
    m_root->renderAll(renderEngine);
}

void mage::Scene::preInit()
{
    m_root = createRef<mage::GameObject>();
}


