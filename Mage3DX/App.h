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



#include "Display.h"
#include "Timer.h"

#include "ImguiManager.h"
#include "IRenderable.h"
#include "Camera.h"
#include "PointLight.h"

namespace mage
{
    class App
    {
    public:
        App(int width, int height, const char* title);
        virtual ~App();
        int run();
        inline void stop() { m_running = false; }
    private:
        void runFrame();

        void spawnWindows();

        Display m_display;
        Camera m_camera;
        PointLight m_light;

        Timer m_timer;
        Timer m_performanceTimer;
        bool m_running;
        float globalSpeed = 1.0f;
        list<UniquePtr<IRenderable>> m_renderables;
        static constexpr size_t NUM_RENDERS = 180;

        list<class Box*> m_boxes;
        std::optional<int> m_comboBoxIndex;
        std::set<int> m_boxIds;
    };

}


#endif //MAGE3DX_APP_H
