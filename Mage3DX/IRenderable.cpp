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
//#include "pch.h" -intellisense works better with force include being used
#include "IRenderable.h"
#include "GraphicsException.h"
#include "IndexBuffer.h"


void IRenderable::render(Graphics& gfx) const noexcept(!MAGE_DEBUG)
{
    for(const auto& b : mBindables)
    {
        b->bind(gfx);
    }
    gfx.drawIndexed(mIndexBuffer->getCount());
}

void IRenderable::addBind(SharedPtr<Bindable> bind) noexcept(!MAGE_DEBUG)
{
	if(typeid(*bind) == typeid(IndexBuffer))
	{
        LOG_ASSERT(!mIndexBuffer, "Binding multiple index buffers is not supported");
        mIndexBuffer = &dynamic_cast<IndexBuffer&>(*bind);
	}

    mBindables.push_back(std::move(bind));
}
