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
 * File Name: app.h
 * Date File Created: 9/15/2020 at 2:56 PM
 * Author: Matt
 */

#ifndef MAGE3DX_APP_H
#define MAGE3DX_APP_H

#include "display.h"
#include "timer.h"

namespace mage
{
    class App
    {
    public:
        App(int width, int height, const char* title) : m_display(width, height, title), m_running(true) {}
        virtual ~App() = default;
        int run();
        inline void stop() { m_running = false; }
    private:
        void update();
        Display m_display;
        Timer m_timer;
        bool m_running;
    };

}


#endif //MAGE3DX_APP_H
