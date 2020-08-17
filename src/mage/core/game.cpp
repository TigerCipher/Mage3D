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
 * File Name: game.cpp
 * Date File Created: 8/16/2020 at 10:36 PM
 * Author: Matt
 */

#include "mage/core/game.h"
#include "mage/core/scene.h"

void mage::Game::setCurrentScene(mage::Scene* scene)
{
    m_currentScene = scene;
    m_currentScene->preInit();
    m_currentScene->init();
}

void mage::Game::processInput(mage::Input* input, float delta)
{
    m_currentScene->input(input, delta);
}

void mage::Game::update(float delta)
{
    m_currentScene->update(delta);
}

void mage::Game::render(const mage::RenderEngine* renderEngine)
{
    m_currentScene->render(renderEngine);
}
