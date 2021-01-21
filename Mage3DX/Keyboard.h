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
 * File Name: Keyboard.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once


#include <bitset>

class Keyboard
{
friend class Display;
public:
	class Event
	{
	public:
		enum class Type
		{
			PRESS, RELEASE, INVALID
		};

		Event() noexcept : mType(Type::INVALID),
			mCode(0) { }
		Event(const Type type, const uchar code) noexcept : mType(type),
		                                                    mCode(code) { }


		[[nodiscard]] bool isPress() const noexcept { return mType == Type::PRESS; }
		[[nodiscard]] bool isRelease() const noexcept { return mType == Type::RELEASE; }
		[[nodiscard]] bool isValid() const noexcept { return mType != Type::INVALID; }

		[[nodiscard]] uchar getCode() const noexcept { return mCode; }

	private:
		Type mType;
		uchar mCode;
	};

	Keyboard() : mPressCounts(NUM_KEYS) {}
	virtual ~Keyboard() = default;
	Keyboard(const Keyboard& rhs) = delete;
	Keyboard& operator=(const Keyboard& rhs) = delete;

	[[nodiscard]] bool isPressed(uchar keycode) const noexcept;
	[[nodiscard]] bool isPressedOnce(uchar keycode) const noexcept;
	Event readKey() noexcept;
	[[nodiscard]] bool isKeyBufferEmpty() const noexcept;
	void clearKeyBuffer() noexcept;


	char readChar() noexcept;
	[[nodiscard]] bool isCharBufferEmpty() const noexcept;
	void clearCharBuffer() noexcept;
	void clearBuffers() noexcept;

	void enableAutoRepeat() noexcept;
	void disableAutoRepeat() noexcept;
	[[nodiscard]] bool isAutoRepeat() const noexcept;

private:

	void onKeyPressed(uchar keycode) noexcept;
	void onKeyReleased(uchar keycode) noexcept;
	void onChar(char c) noexcept;

	void clearState() noexcept;

	template<typename T>
	static void trimBuffer(std::queue<T>& buffer) noexcept;


	static constexpr uint NUM_KEYS = 256;
	static constexpr uint BUFFER_SIZE = 16;

	bool mAutoRepeat = false;
	//mutable ushort mPressCount = 0;
	mutable list<int> mPressCounts;
	std::bitset<NUM_KEYS> mKeyStates;
	std::queue<Event> mKeyBuffer;
	std::queue<char> mCharBuffer;
};

