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
//#include "pch.h" -intellisense works better with force include being used
#include "Keyboard.h"


bool Keyboard::isPressed(uchar keycode) const noexcept
{
    return mKeyStates[ keycode ];
}

bool Keyboard::isPressedOnce(uchar keycode) const noexcept
{
    bool down = mKeyStates[keycode];
    if(down && mPressCount++ == 0) return down;
    else if(!down) mPressCount = 0;
    return false;
}


Keyboard::Event Keyboard::readKey() noexcept
{
    if (!mKeyBuffer.empty())
    {
        Event e = mKeyBuffer.front();
        mKeyBuffer.pop();
        return e;
    }
    return Keyboard::Event();
}

bool Keyboard::isKeyBufferEmpty() const noexcept
{
    return mKeyBuffer.empty();
}

void Keyboard::clearKeyBuffer() noexcept
{
    mKeyBuffer = std::queue<Event>();
}

char Keyboard::readChar() noexcept
{
    if (!mCharBuffer.empty())
    {
        uchar c = mCharBuffer.front();
        mCharBuffer.pop();
        return c;
    }
    return 0;
}

bool Keyboard::isCharBufferEmpty() const noexcept
{
    return mCharBuffer.empty();
}

void Keyboard::clearCharBuffer() noexcept
{
    mCharBuffer = std::queue<char>();
}

void Keyboard::clearBuffers() noexcept
{
    clearKeyBuffer();
    clearCharBuffer();
}

void Keyboard::enableAutoRepeat() noexcept
{
    mAutoRepeat = true;
}

void Keyboard::disableAutoRepeat() noexcept
{
    mAutoRepeat = false;
}

bool Keyboard::isAutoRepeat() const noexcept
{
    return mAutoRepeat;
}

void Keyboard::onKeyPressed(uchar keycode) noexcept
{
    mKeyStates[ keycode ] = true;
    mKeyBuffer.push(Event(Event::Type::PRESS, keycode));
    trimBuffer(mKeyBuffer);
}

void Keyboard::onKeyReleased(uchar keycode) noexcept
{
    mKeyStates[ keycode ] = false;
    mKeyBuffer.push(Event(Event::Type::RELEASE, keycode));
    trimBuffer(mKeyBuffer);
}

void Keyboard::onChar(char c) noexcept
{
    mCharBuffer.push(c);
    trimBuffer(mCharBuffer);
}

void Keyboard::clearState() noexcept
{
    mKeyStates.reset();
}

template<typename T>
void Keyboard::trimBuffer(std::queue<T>& buffer) noexcept
{
    while(buffer.size() > BUFFER_SIZE)
    {
        buffer.pop();
    }
}
