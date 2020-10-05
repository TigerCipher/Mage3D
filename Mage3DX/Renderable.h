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
 * File Name: Renderable.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once


#include "IRenderable.h"
#include "IndexBuffer.h"


template<class T>
class Renderable : public IRenderable
{
protected:
	static bool isInitialized() noexcept { return !staticBinds.empty(); }

	static void addStaticBind(UniquePtr<Bindable> bindable) noexcept(!MAGE_DEBUG)
	{
		assert("MUST use addStaticIndexBuffer for IndexBuffer types" && typeid(*bindable) != typeid(IndexBuffer));
		staticBinds.push_back(std::move(bindable));
	}

	void addStaticIndexBuffer(UniquePtr<IndexBuffer> ibuf) noexcept(!MAGE_DEBUG)
	{
		assert("Do not add a second IndexBuffer" && !m_indexBuffer);
		m_indexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}

	void setIndexStatic() noexcept(!MAGE_DEBUG)
	{
		assert("Do not add a second IndexBuffer" && !m_indexBuffer);
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				m_indexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer from static bindables" && m_indexBuffer);
	}

private:
	[[nodiscard]] const list<UniquePtr<Bindable> >& getStaticBinds() const noexcept override { return staticBinds; }

	static list<UniquePtr<Bindable> > staticBinds;
};


template<class T>
list<UniquePtr<Bindable> > Renderable<T>::staticBinds;
