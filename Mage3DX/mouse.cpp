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
 * File Name: mouse.cpp
 * Date File Created: 9/13/2020 at 2:39 PM
 * Author: Matt
 */

#include "mouse.h"


mage::Mouse::Event mage::Mouse::read() noexcept
{
    if(!m_buffer.empty())
    {
        Event e = m_buffer.front();
        m_buffer.pop();
        return e;
    }
    return mage::Mouse::Event();
}

void mage::Mouse::clear() noexcept
{
    m_buffer = std::queue<Event>();
}

void mage::Mouse::onMouseMove(int x, int y) noexcept
{
    m_xPos = x;
    m_yPos = y;
    m_buffer.push(Event(Event::Type::MOVE, *this));
    trim();
}

void mage::Mouse::onLeftPressed(int x, int y) noexcept
{
    m_left = true;
    m_buffer.push(Event(Event::Type::LEFT_PRESS, *this));
    trim();
}

void mage::Mouse::onLeftReleased(int x, int y) noexcept
{
    m_left = false;
    m_buffer.push(Event(Event::Type::LEFT_RELEASE, *this));
    trim();
}

void mage::Mouse::onRightPressed(int x, int y) noexcept
{
    m_right = true;
    m_buffer.push(Event(Event::Type::RIGHT_PRESS, *this));
    trim();
}

void mage::Mouse::onRightReleased(int x, int y) noexcept
{
    m_right = false;
    m_buffer.push(Event(Event::Type::RIGHT_RELEASE, *this));
    trim();
}

void mage::Mouse::onWheelUp(int x, int y) noexcept
{
    m_buffer.push(Event(Event::Type::WHEEL_UP, *this));
    trim();
}

void mage::Mouse::onWheelDown(int x, int y) noexcept
{
    m_buffer.push(Event(Event::Type::WHEEL_DOWN, *this));
    trim();
}

void mage::Mouse::onMouseEnter() noexcept
{
    m_inWindow = true;
    m_buffer.push(Event(Event::Type::ENTER, *this));
    trim();
}

void mage::Mouse::onMouseLeave() noexcept
{
    m_inWindow = false;
    m_buffer.push(Event(Event::Type::LEAVE, *this));
    trim();
}

void mage::Mouse::onWheelDelta(int x, int y, int delta) noexcept
{
    m_wheelDelta += delta;
    while(m_wheelDelta >= WHEEL_DELTA)
    {
        m_wheelDelta -= WHEEL_DELTA;
        onWheelUp(x, y);
    }
    while(m_wheelDelta <= -WHEEL_DELTA)
    {
        m_wheelDelta += WHEEL_DELTA;
        onWheelDown(x, y);
    }
}

void mage::Mouse::trim() noexcept
{
    while(m_buffer.size() > BUFFER_SIZE)
    {
        m_buffer.pop();
    }
}
