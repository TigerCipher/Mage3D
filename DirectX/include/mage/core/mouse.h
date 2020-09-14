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
 * File Name: mouse.h
 * Date File Created: 9/13/2020 at 2:39 PM
 * Author: Matt
 */

#ifndef MAGE3DX_MOUSE_H
#define MAGE3DX_MOUSE_H


#include "mage/common.h"
#include <queue>


namespace mage
{
    class Mouse
    {
        friend class Display;
    public:
        class Event
        {
        public:
            enum class Type
            {
                LEFT_PRESS, LEFT_RELEASE, RIGHT_PRESS, RIGHT_RELEASE, WHEEL_UP, WHEEL_DOWN, MOVE, ENTER, LEAVE, INVALID
            };

            Event() noexcept:
                    m_type(Type::INVALID),
                    m_left(false),
                    m_right(false),
                    m_xPos(0),
                    m_yPos(0) { }

            Event(Type type, const Mouse& parent) noexcept:
                    m_type(type),
                    m_left(parent.m_left),
                    m_right(parent.m_right),
                    m_xPos(parent.m_xPos),
                    m_yPos(parent.m_yPos) { }

            [[nodiscard]] bool isValid() const noexcept { return m_type != Type::INVALID; }

            [[nodiscard]] Type getType() const noexcept { return m_type; }

            [[nodiscard]] std::pair<int, int> getPos() const noexcept { return { m_xPos, m_yPos }; }

            [[nodiscard]] int getX() const noexcept { return m_xPos; }

            [[nodiscard]] int getY() const noexcept { return m_yPos; }

            [[nodiscard]] bool isLeftPressed() const noexcept { return m_left; }

            [[nodiscard]] bool isRightPressed() const noexcept { return m_right; }

        private:
            Type m_type;
            bool m_left;
            bool m_right;
            int m_xPos;
            int m_yPos;
        };
    public:
        Mouse() = default;
        virtual ~Mouse() = default;
        Mouse(const Mouse& rhs) = delete;
        Mouse& operator=(const Mouse& rhs) = delete;

        [[nodiscard]] std::pair<int, int> getPos() const noexcept { return { m_xPos, m_yPos }; }

        [[nodiscard]] int getX() const noexcept { return m_xPos; }

        [[nodiscard]] int getY() const noexcept { return m_yPos; }

        [[nodiscard]] bool isLeftPressed() const noexcept { return m_left; }

        [[nodiscard]] bool isRightPressed() const noexcept { return m_right; }
        [[nodiscard]] bool isInWindow() const noexcept { return m_inWindow; }

        Event read() noexcept;
        [[nodiscard]] bool isEmpty() const noexcept { return m_buffer.empty(); }

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

        int m_xPos;
        int m_yPos;
        bool m_left = false;
        bool m_right = false;
        bool m_inWindow = false;
        int m_wheelDelta = 0;
        std::queue<Event> m_buffer;
    };

}


#endif //MAGE3DX_MOUSE_H
