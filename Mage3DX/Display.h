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
 * File Name: Display.h
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#pragma once

#include "Keyboard.h"
#include "DisplayException.h"
#include "Graphics.h"
#include "winwrapper.h"
#include "Mouse.h"

class Display
{
public:
	Display(int width, int height, const char* title);
	virtual ~Display();
	Display(const Display& disp) = delete;
	Display& operator=(const Display& disp) = delete;

	void setTitle(const std::string& title);

	/// <summary>
	/// Toggles the cursor visibility
	/// </summary>
	/// <param name="mode">default -1. Value less than 0 will toggle. 0 will set to hidden. 1 will set to visible</param>
	void toggleCursor(int16 mode = -1) noexcept;
	
	static std::optional<int> processMessages() noexcept;

	Graphics& getGraphics();

	[[nodiscard]] int getWidth() const noexcept { return mWidth; }
	[[nodiscard]] int getHeight() const noexcept { return mHeight; }

	[[nodiscard]] float getAspectRatio() const noexcept { return mAspectRatio; }

	[[nodiscard]] bool isCursorEnabled() const noexcept { return mCursor; }

	Keyboard keyboard;
	Mouse mouse;
protected:
private:

	static void showCursor() noexcept;
	static void hideCursor() noexcept;
	void trapCursor() const noexcept;
	static void freeCursor() noexcept;

	static LRESULT CALLBACK handleMessageSetup(HWND hWnd, UINT msg,
	                                           WPARAM wParam, LPARAM lParam) noexcept;

	static LRESULT CALLBACK handleMessageIntermediate(HWND hWnd, UINT msg,
	                                                  WPARAM wParam, LPARAM lParam) noexcept;

	LRESULT handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	int mWidth;
	int mHeight;
	float mAspectRatio;

	bool mCursor;
	
	HWND mHwnd;
	UniquePtr<Graphics> mGfx;
	list<BYTE> mRawBuffer;

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
		HINSTANCE mHInst;
	};
};
