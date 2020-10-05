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
 * File Name: IRenderable.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "Graphics.h"
#include "MathHelper.h"


class Bindable;

class IRenderable
{
template<class T>
friend class Renderable;

public:
	IRenderable() = default;
	virtual ~IRenderable() = default;
	IRenderable(const IRenderable& rhs) = delete;
	IRenderable& operator=(const IRenderable& rhs) = delete;

	[[nodiscard]] virtual mat4f getTransformMatrix() const noexcept = 0;

	void render(Graphics& gfx) const noexcept(!MAGE_DEBUG);
	virtual void update(float delta) noexcept { }

protected:
	void addBind(UniquePtr<Bindable> bindable) noexcept(!MAGE_DEBUG);
	void addIndexBuffer(UniquePtr<class IndexBuffer> ibuf) noexcept;

	template<class T>
	T* queryBindable() noexcept
	{
		for (auto& pixelBuffer : m_bindables)
		{
			if(auto pb = dynamic_cast<T*>(pixelBuffer.get()))
				return pb;
		}

		return nullptr;
	}

private:
	[[nodiscard]] virtual const list<UniquePtr<Bindable> >& getStaticBinds() const noexcept = 0;

	const class IndexBuffer* m_indexBuffer = nullptr;
	list<UniquePtr<Bindable> > m_bindables{ };
};



