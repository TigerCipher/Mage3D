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
 * File Name: vertexshader.h
 * Date File Created: 9/20/2020 at 10:48 PM
 * Author: Matt
 */

#ifndef MAGE3DX_VERTEX_SHADER_H
#define MAGE3DX_VERTEX_SHADER_H


//#include "pch.h"
#include "bindable.h"


namespace mage
{
    class VertexShader : public Bindable
    {
    public:
        VertexShader(Graphics& gfx, const std::wstring& path);
        void bind(Graphics& gfx) noexcept override;
        [[nodiscard]] ID3DBlob* getBytecode() const noexcept;

    protected:
        COMptr<ID3DBlob> m_bytecode;
        COMptr<ID3D11VertexShader> m_shader;
    };

}// namespace mage


#endif//MAGE3DX_VERTEX_SHADER_H
