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
 * File Name: keyboard.cpp
 * Date File Created: 9/13/2020 at 12:51 PM
 * Author: Matt
 */

#include "mage/core/keyboard.h"

bool mage::Keyboard::isPressed(uchar keycode) const noexcept
{
    return m_keyStates[ keycode ];
}

mage::Keyboard::Event mage::Keyboard::readKey() noexcept
{
    if (!m_keyBuffer.empty())
    {
        Event e = m_keyBuffer.front();
        m_keyBuffer.pop();
        return e;
    }
    return mage::Keyboard::Event();
}

bool mage::Keyboard::isKeyBufferEmpty() const noexcept
{
    return m_keyBuffer.empty();
}

void mage::Keyboard::clearKeyBuffer() noexcept
{
    m_keyBuffer = std::queue<Event>();
}

char mage::Keyboard::readChar() noexcept
{
    if (!m_charBuffer.empty())
    {
        uchar c = m_charBuffer.front();
        m_charBuffer.pop();
        return c;
    }
    return 0;
}

bool mage::Keyboard::isCharBufferEmpty() const noexcept
{
    return m_charBuffer.empty();
}

void mage::Keyboard::clearCharBuffer() noexcept
{
    m_charBuffer = std::queue<char>();
}

void mage::Keyboard::clearBuffers() noexcept
{
    clearKeyBuffer();
    clearCharBuffer();
}

void mage::Keyboard::enableAutoRepeat() noexcept
{
    m_autoRepeat = true;
}

void mage::Keyboard::disableAutoRepeat() noexcept
{
    m_autoRepeat = false;
}

bool mage::Keyboard::isAutoRepeat() const noexcept
{
    return m_autoRepeat;
}

void mage::Keyboard::onKeyPressed(uchar keycode) noexcept
{
    m_keyStates[ keycode ] = true;
    m_keyBuffer.push(Event(Event::Type::PRESS, keycode));
    trimBuffer(m_keyBuffer);
}

void mage::Keyboard::onKeyReleased(uchar keycode) noexcept
{
    m_keyStates[ keycode ] = false;
    m_keyBuffer.push(Event(Event::Type::RELEASE, keycode));
    trimBuffer(m_keyBuffer);
}

void mage::Keyboard::onChar(char c) noexcept
{
    m_charBuffer.push(c);
    trimBuffer(m_charBuffer);
}

void mage::Keyboard::clearState() noexcept
{
    m_keyStates.reset();
}

template<typename T>
void mage::Keyboard::trimBuffer(std::queue<T>& buffer) noexcept
{
    while(buffer.size() > BUFFER_SIZE)
    {
        buffer.pop();
    }
}
