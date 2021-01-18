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
 * File Name: Mouse.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

class Mouse
{
friend class Display;
public:
	class Event
	{
	public:
		enum class Type
		{
			LEFT_PRESS,
			LEFT_RELEASE,
			RIGHT_PRESS,
			RIGHT_RELEASE,
			WHEEL_UP,
			WHEEL_DOWN,
			MOVE, ENTER,
			LEAVE,
			INVALID
		};

		Event() noexcept:
			mType(Type::INVALID),
			mLeft(false),
			mRight(false),
			mXPos(0),
			mYPos(0) { }

		Event(const Type type, const Mouse& parent) noexcept:
			mType(type),
			mLeft(parent.mLeft),
			mRight(parent.mRight),
			mXPos(parent.mXPos),
			mYPos(parent.mYPos) { }

		[[nodiscard]] bool isValid() const noexcept { return mType != Type::INVALID; }

		[[nodiscard]] Type getType() const noexcept { return mType; }

		[[nodiscard]] std::pair<int, int> getPos() const noexcept { return { mXPos, mYPos }; }

		[[nodiscard]] int getX() const noexcept { return mXPos; }

		[[nodiscard]] int getY() const noexcept { return mYPos; }

		[[nodiscard]] bool isLeftPressed() const noexcept { return mLeft; }

		[[nodiscard]] bool isRightPressed() const noexcept { return mRight; }

	private:
		Type mType;
		bool mLeft;
		bool mRight;
		int mXPos;
		int mYPos;
	};
public:
	Mouse() = default;
	virtual ~Mouse() = default;
	Mouse(const Mouse& rhs) = delete;
	Mouse& operator=(const Mouse& rhs) = delete;

	[[nodiscard]] std::pair<int, int> getPos() const noexcept { return { mXPos, mYPos }; }

	[[nodiscard]] int getX() const noexcept { return mXPos; }

	[[nodiscard]] int getY() const noexcept { return mYPos; }

	[[nodiscard]] bool isLeftPressed() const noexcept { return mLeft; }

	[[nodiscard]] bool isRightPressed() const noexcept { return mRight; }
	[[nodiscard]] bool isInWindow() const noexcept { return mInWindow; }

	Event read() noexcept;
	[[nodiscard]] bool isEmpty() const noexcept { return mBuffer.empty(); }

	void clear() noexcept;


private:

	void onMouseMove(int x, int y) noexcept;
	void onLeftPressed(int x, int y) noexcept;
	void onLeftReleased(int x, int y) noexcept;
	void onRightPressed(int x, int y) noexcept;
	void onRightReleased(int x, int y) noexcept;
	void onWheelUp(int x, int y) noexcept;
	void onWheelDown(int x, int y) noexcept;
	void onMouseEnter() noexcept;
	void onMouseLeave() noexcept;
	void onWheelDelta(int x, int y, int delta) noexcept;
	void trim() noexcept;

	static constexpr uint BUFFER_SIZE = 16;

	int mXPos;
	int mYPos;
	bool mLeft = false;
	bool mRight = false;
	bool mInWindow = false;
	int mWheelDelta = 0;
	std::queue<Event> mBuffer;
};
