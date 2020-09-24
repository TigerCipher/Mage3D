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
 * File Name: renderable.h
 * Date File Created: 9/20/2020 at 10:28 PM
 * Author: Matt
 */

#ifndef MAGE3DX_IRENDERABLE_H
#define MAGE3DX_IRENDERABLE_H


//#include "pch.h"
#include "graphics.h"
#include "mage/core/math_helper.h"


namespace mage
{
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
        virtual void update(float delta) noexcept = 0;

    protected:
        void addBindable(UniquePtr<Bindable> bindable) noexcept(!MAGE_DEBUG);
        void addIndexBuffer(UniquePtr<class IndexBuffer> ibuf) noexcept;

    private:
        [[nodiscard]] virtual const list<UniquePtr<Bindable>>& getStaticBinds() const noexcept = 0;

        const class IndexBuffer* m_indexBuffer = nullptr;
        list<UniquePtr<Bindable>> m_bindables;
    };


}// namespace mage


#endif//MAGE3DX_IRENDERABLE_H
