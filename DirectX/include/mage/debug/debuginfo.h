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
 * File Name: debuginfo.h
 * Date File Created: 9/16/2020 at 11:05 PM
 * Author: Matt
 */

#ifndef MAGE3DX_DEBUGINFO_H
#define MAGE3DX_DEBUGINFO_H

//#include "pch.h"
#include <dxgidebug.h>

namespace mage
{
    class DebugInfo
    {
    public:
        DebugInfo();
        virtual ~DebugInfo() = default;
        DebugInfo(const DebugInfo& rhs) = delete;
        DebugInfo& operator=(const DebugInfo& rhs) = delete;

        void set() noexcept;
        [[nodiscard]] list<std::string> getMessages() const;
    private:
        ulonglong m_next = 0;
        COMptr<IDXGIInfoQueue> m_infoQueue{};
    };

}


#endif //MAGE3DX_DEBUGINFO_H
