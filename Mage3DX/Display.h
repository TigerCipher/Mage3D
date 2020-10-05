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
 * File Name: display.h
 * Date File Created: 9/27/2020 at 3:31 PM
 * Author: Matt
 */
#pragma once

#include "Keyboard.h"
#include "DisplayException.h"
#include "Graphics.h"
#include "winwrapper.h"
#include "Mouse.h"

namespace mage
{
	class Display
	{
		public:
			Display(int width, int height, const char* title);
			virtual ~Display();
			Display(const Display& disp) = delete;
			Display& operator=(const Display& disp) = delete;

			void setTitle(const std::string& title);
			static std::optional<int> processMessages() noexcept;

			Graphics& getGraphics();

			[[nodiscard]] inline int getWidth() const noexcept { return m_width; }
			[[nodiscard]] inline int getHeight() const noexcept { return m_height; }

			[[nodiscard]] inline float getAspectRatio() const noexcept { return m_aspectRatio; }

			Keyboard keyboard;
			Mouse mouse;
		protected:
		private:

			static LRESULT CALLBACK handleMessageSetup(HWND hWnd, UINT msg,
			                                           WPARAM wParam, LPARAM lParam) noexcept;

			static LRESULT CALLBACK handleMessageIntermediate(HWND hWnd, UINT msg,
			                                                  WPARAM wParam, LPARAM lParam) noexcept;

			LRESULT handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

			int m_width;
			int m_height;
			float m_aspectRatio;
			HWND m_hwnd;
			UniquePtr<Graphics> m_gfx;

			class Window
			{
				public:
					static const char* getName() noexcept;
					static HINSTANCE getInstance() noexcept;
					Window(const Window& win) = delete;
					Window& operator=(const Window& win) = delete;
				private:
					Window() noexcept;
					virtual ~Window();


					static constexpr const char* sWinClassName = "Mage3DXWin";
					static Window sWinClass;
					HINSTANCE m_hInst;
			};
	};
}
