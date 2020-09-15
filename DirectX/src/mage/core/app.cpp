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
 * File Name: app.cpp
 * Date File Created: 9/15/2020 at 2:56 PM
 * Author: Matt
 */

#include "mage/core/app.h"

int mage::App::run()
{
    while(m_running)
    {
        if(const auto exitCode = Display::processMessages())
            return *exitCode;
        update();
    }

    return 0;
}

void mage::App::update()
{
    const float t = m_timer.peek();
    std::ostringstream oss;
    oss << "Time Elapsed: " << std::setprecision(1) << std::fixed << t << "s";
    m_display.setTitle(oss.str());
}
