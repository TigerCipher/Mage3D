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
    LOG_INFO("Main loop beginning");
    while (m_running)
    {
        if (const auto exitCode = Display::processMessages())
            return *exitCode;
        update();
    }
    return 0;
}

void mage::App::update()
{
    float delta = m_timer.markPoint();
    m_display.getGraphics().clear(0.07f, 0, 0.12f);

    for(auto& b : m_boxes)
    {
        b->update(delta);
        b->render(m_display.getGraphics());
    }

    m_display.getGraphics().swap();
}

mage::App::App(int width, int height, const char* title) :
        m_display(width, height, title),
        m_running(true)
{
    std::mt19937 rng(std::random_device { }());
    std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
    std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
    std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
    std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

    for (auto i = 0; i < 80; i++)
    {
        m_boxes.push_back(createScope<Box>(
                m_display.getGraphics(), rng, adist,
                ddist, odist, rdist
                                          ));
    }
    m_display.getGraphics()
             .setProjection(dx::XMMatrixPerspectiveLH(1.0f, m_display.getAspectRatio(), 0.5f, 40.0f));
}
