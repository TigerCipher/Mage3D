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
 * File Name: gameobject.cpp
 * Date File Created: 8/14/2020 at 9:17 PM
 * Author: Matt
 */

#include "mage/core/gameobject.h"
#include "mage/core/gamecomponent.h"

mage::GameObject::~GameObject()
{
    for (auto& component : m_components)
        delete component;
    for (auto& child : m_children)
        delete child;
}

mage::GameObject* mage::GameObject::addChild(mage::GameObject* child)
{
    m_children.push_back(child);
    child->getTransform().setParent(&m_transform);
    return this;
}

mage::GameObject* mage::GameObject::addComponent(mage::GameComponent* component)
{
    m_components.push_back(component);
    component->setParent(this);
    return this;
}

void mage::GameObject::inputAll(mage::Input* _input, float delta)
{
    input(_input, delta);
    for (const auto& child : m_children)
        child->inputAll(_input, delta);
}

void mage::GameObject::updateAll(float delta)
{
    update(delta);
    for (const auto& child : m_children)
        child->updateAll(delta);
}

void mage::GameObject::renderAll(const RenderEngine* renderEngine)
{
    render(renderEngine);
    for (const auto& child : m_children)
        child->renderAll(renderEngine);
}

list<mage::GameObject*> mage::GameObject::getAttachedChildren()
{
    list<GameObject*> ret;
    for (const auto& child : m_children)
    {
        list<GameObject*> children = child->getAttachedChildren();
        ret.insert(ret.end(), children.begin(), children.end());
    }
    ret.push_back(this);
    return ret;
}

void mage::GameObject::input(mage::Input* _input, float delta)
{
    m_transform.update();
    for (const auto& component : m_components)
        component->input(_input, delta);
}

void mage::GameObject::update(float delta)
{
    for (const auto& component : m_components)
        component->update(delta);
}

void mage::GameObject::render(const RenderEngine* renderEngine)
{
    for (const auto& component : m_components)
        component->render(renderEngine);
}


