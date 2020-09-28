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
 * File Name: renderable.cpp
 * Date File Created: 9/20/2020 at 10:28 PM
 * Author: Matt
 */

#include "IRenderable.h"
#include "GraphicsException.h"
#include "IndexBuffer.h"
#include <cassert>
#include <typeinfo>


void mage::IRenderable::render(mage::Graphics& gfx) const noexcept(!MAGE_DEBUG)
{
    for(auto& b : m_bindables)
    {
        b->bind(gfx);
    }
    for(auto& b : getStaticBinds())
    {
        b->bind(gfx);
    }
    gfx.drawIndexed(m_indexBuffer->getCount());
}


void mage::IRenderable::addBindable(UniquePtr<mage::Bindable> bindable) noexcept(!MAGE_DEBUG)
{
    assert("MUST use addIndexBuffer when binding an IndexBuffer" && typeid(*bindable) != typeid(IndexBuffer));
    m_bindables.push_back(std::move(bindable));
}

void mage::IRenderable::addIndexBuffer(UniquePtr<struct IndexBuffer> ibuf) noexcept
{
    assert("Do not add a second IndexBuffer" && !m_indexBuffer);
    m_indexBuffer = ibuf.get();
    m_bindables.push_back(std::move(ibuf));
}
