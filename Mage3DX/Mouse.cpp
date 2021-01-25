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
//#include "pch.h" -intellisense works better with force include being used
#include "Mouse.h"


std::optional<Mouse::RawDelta> Mouse::readRawDelta() noexcept
{
    if (mRawDeltaBuffer.empty()) return std::nullopt;
    const RawDelta rd = mRawDeltaBuffer.front();
    mRawDeltaBuffer.pop();
    return rd;
}

Mouse::Event Mouse::read() noexcept
{
    if(!mBuffer.empty())
    {
        Event e = mBuffer.front();
        mBuffer.pop();
        return e;
    }
    return Mouse::Event();
}

void Mouse::clear() noexcept
{
    mBuffer = std::queue<Event>();
}

void Mouse::toggleRawInput(const int16 mode)
{
    if (mode < 0) mRawInput = !mRawInput;
    else if (mode == 0) mRawInput = false;
    else mRawInput = true;
}

void Mouse::onRawDelta(int dx, int dy)
{
    mRawDeltaBuffer.push({ dx, dy });
    trimRawInputBuffer();
}

void Mouse::onMouseMove(int x, int y)
{
    mXPos = x;
    mYPos = y;
    mBuffer.push(Event(Event::Type::MOVE, *this));
    trim();
}

void Mouse::onLeftPressed(int x, int y)
{
    mLeft = true;
    mBuffer.push(Event(Event::Type::LEFT_PRESS, *this));
    trim();
}

void Mouse::onLeftReleased(int x, int y)
{
    mLeft = false;
    mBuffer.push(Event(Event::Type::LEFT_RELEASE, *this));
    trim();
}

void Mouse::onRightPressed(int x, int y)
{
    mRight = true;
    mBuffer.push(Event(Event::Type::RIGHT_PRESS, *this));
    trim();
}

void Mouse::onRightReleased(int x, int y)
{
    mRight = false;
    mBuffer.push(Event(Event::Type::RIGHT_RELEASE, *this));
    trim();
}

void Mouse::onWheelUp(int x, int y)
{
    mBuffer.push(Event(Event::Type::WHEEL_UP, *this));
    trim();
}

void Mouse::onWheelDown(int x, int y)
{
    mBuffer.push(Event(Event::Type::WHEEL_DOWN, *this));
    trim();
}

void Mouse::onMouseEnter()
{
    mInWindow = true;
    mBuffer.push(Event(Event::Type::ENTER, *this));
    trim();
}

void Mouse::onMouseLeave()
{
    mInWindow = false;
    mBuffer.push(Event(Event::Type::LEAVE, *this));
    trim();
}

void Mouse::onWheelDelta(int x, int y, int delta)
{
    mWheelDelta += delta;
    while(mWheelDelta >= WHEEL_DELTA)
    {
        mWheelDelta -= WHEEL_DELTA;
        onWheelUp(x, y);
    }
    while(mWheelDelta <= -WHEEL_DELTA)
    {
        mWheelDelta += WHEEL_DELTA;
        onWheelDown(x, y);
    }
}

void Mouse::trim() noexcept
{
    while(mBuffer.size() > BUFFER_SIZE)
    {
        mBuffer.pop();
    }
}

void Mouse::trimRawInputBuffer() noexcept
{
	while(mRawDeltaBuffer.size() > BUFFER_SIZE)
	{
        mRawDeltaBuffer.pop();
	}
}
