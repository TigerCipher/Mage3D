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


	struct RawDelta
	{
		int x;
		int y;
	};
public:
	Mouse() = default;
	virtual ~Mouse() = default;
	Mouse(const Mouse& rhs) = delete;
	Mouse& operator=(const Mouse& rhs) = delete;

	[[nodiscard]] std::pair<int, int> getPos() const noexcept { return { mXPos, mYPos }; }

	std::optional<RawDelta> readRawDelta() noexcept;

	[[nodiscard]] int getX() const noexcept { return mXPos; }

	[[nodiscard]] int getY() const noexcept { return mYPos; }

	[[nodiscard]] bool isLeftPressed() const noexcept { return mLeft; }

	[[nodiscard]] bool isRightPressed() const noexcept { return mRight; }
	[[nodiscard]] bool isInWindow() const noexcept { return mInWindow; }

	[[nodiscard]] bool isRawInputEnabled() const noexcept { return mRawInput; }

	Event read() noexcept;
	[[nodiscard]] bool isEmpty() const noexcept { return mBuffer.empty(); }

	void clear() noexcept;

	void toggleRawInput(const int16 mode = -1);


private:

	void onRawDelta(int dx, int dy);
	void onMouseMove(int x, int y);
	void onLeftPressed(int x, int y);
	void onLeftReleased(int x, int y);
	void onRightPressed(int x, int y);
	void onRightReleased(int x, int y);
	void onWheelUp(int x, int y);
	void onWheelDown(int x, int y);
	void onMouseEnter();
	void onMouseLeave();
	void onWheelDelta(int x, int y, int delta);
	void trim() noexcept;
	void trimRawInputBuffer() noexcept;

	static constexpr uint BUFFER_SIZE = 16;

	int mXPos;
	int mYPos;
	bool mLeft = false;
	bool mRight = false;
	bool mInWindow = false;
	int mWheelDelta = 0;
	bool mRawInput = true;
	std::queue<Event> mBuffer;
	std::queue<RawDelta> mRawDeltaBuffer;
};
