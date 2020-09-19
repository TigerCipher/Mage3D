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
 * File Name: debugmessagemap.h
 * Date File Created: 9/10/2020 at 11:50 PM
 * Author: Matt
 */

#ifndef MAGE3DX_DEBUGMESSAGEMAP_H
#define MAGE3DX_DEBUGMESSAGEMAP_H

#include "pch.h"

namespace mage
{
    class DebugMessageMap
    {
    public:
        DebugMessageMap() noexcept;
        virtual ~DebugMessageMap() = default;

        std::string operator() (DWORD msg, LPARAM lParam, WPARAM wParam) const noexcept;

    protected:
    private:
        std::unordered_map<DWORD, std::string> m_map;
    };

}


#endif //MAGE3DX_DEBUGMESSAGEMAP_H
