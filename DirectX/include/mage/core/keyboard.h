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
 * File Name: keyboard.h
 * Date File Created: 9/13/2020 at 12:51 PM
 * Author: Matt
 */

#ifndef MAGE3DX_KEYBOARD_H
#define MAGE3DX_KEYBOARD_H

//#include "pch.h"
#include <bitset>

namespace mage
{
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

            Event() noexcept : m_type(Type::INVALID), m_code(0) {}
            Event(Type type, uchar code) noexcept : m_type(type), m_code(code) {}


            [[nodiscard]] bool isPress() const noexcept { return m_type == Type::PRESS; }
            [[nodiscard]] bool isRelease() const noexcept { return m_type == Type::RELEASE; }
            [[nodiscard]] bool isValid() const noexcept { return m_type != Type::INVALID; }

            [[nodiscard]] uchar getCode() const noexcept { return m_code; }

        private:
            Type m_type;
            uchar m_code;
        };
    public:
        Keyboard() = default;
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

        bool m_autoRepeat = false;
        mutable ushort m_pressCount = 0;
        std::bitset<NUM_KEYS> m_keyStates;
        std::queue<Event> m_keyBuffer;
        std::queue<char> m_charBuffer;

    };

}


#endif //MAGE3DX_KEYBOARD_H
