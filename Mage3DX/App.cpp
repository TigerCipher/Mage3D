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
 * File Name: App.cpp
 * Date File Created: 10/1/2020 at 11:38 PM
 * Author: Matt
 */
#include "App.h"
#include "GDIPlusManager.h"
#include "Texture.h"
#include "ImguiManager.h"
#include "Bindables.h"


GDIPlusManager gGdip;

App::App(const int width, const int height, const char* title) :
	mDisplay(width, height, title),
	mLight(mDisplay.getGraphics()),
	mRunning(true),
	mNano(mDisplay.getGraphics(), "assets\\models\\nanosuit.obj"),
	mNano2(mDisplay.getGraphics(), "assets\\models\\nanosuit.obj")
{

	mDisplay.getGraphics().setProjection(dx::XMMatrixPerspectiveLH(1.0f,
		mDisplay.getAspectRatio(),
		0.5f, 40.0f));
	mDisplay.toggleCursor(0);

}

App::~App()
{
	if (mRunning)
		stop();
}

int App::run()
{
	LOG_INFO("Main loop beginning");
	while (mRunning)
	{
		if (const auto exitCode = Display::processMessages())
			return *exitCode;
		runFrame();
	}
	ImguiManager::stop();
	return 0;
}

void App::stop()
{
	mRunning = false;
}

void calculateFrameStatistics(Timer& timer)
{
	static float fps = 0;
	static float avgFps = 0;
	static int frameCount = 0;
	static float prntAvgFps = 0;
	static int prntFrameCount = 0;

	if (ImguiManager::isEnabled())
	{
		frameCount++;
		fps = ImGui::GetIO().Framerate;
		avgFps += fps;
		if (timer.peek() >= 1.0f)
		{
			prntAvgFps = avgFps / static_cast<float>(frameCount);
			avgFps = 0;
			prntFrameCount = frameCount;
			frameCount = 0;
			timer.markPoint();
		}
	}


	IMGUI_BEGIN("Performance Statistics", nullptr, ImGuiWindowFlags_AlwaysAutoResize)
	IMGUI_FUNC(Text("Frame Stats: %.3f ms/frame (%.1f FPS)"));
	IMGUI_FUNC(Text("Average Frame Stats: %.3f ms/frame (%.1f FPS)", 1000.0f / prntAvgFps, prntAvgFps));
	IMGUI_FUNC(TextColored(ImVec4(1, 1, 0, 1), "Processed %i frames", prntFrameCount));
	IMGUI_END
}


void App::runFrame()
{
	const auto delta = mTimer.markPoint() * mGlobalSpeed;

	// TODO Fix input data capture rate
	// Sometimes captures input data too fast and one press is seen as multiple
	if (mDisplay.keyboard.isPressedOnce(VK_NUMPAD5)) ImguiManager::toggle();

	mDisplay.getGraphics().clear(0.07f, 0, 0.12f);
	mDisplay.getGraphics().setCamera(mCamera.getViewMatrix());
	mLight.bind(mDisplay.getGraphics(), mCamera.getViewMatrix());


	mNano.render(mDisplay.getGraphics());
	mNano2.render(mDisplay.getGraphics());
	mLight.render(mDisplay.getGraphics());

	// Sometimes captures input data too fast and one press is seen as multiple
	if (mDisplay.keyboard.isPressedOnce(VK_INSERT))
	{
		mDisplay.toggleCursor();
		mDisplay.mouse.toggleRawInput();
	}

	if(!mDisplay.isCursorEnabled())
	{
		if (mDisplay.keyboard.isPressed('W'))
			mCamera.translate({ 0, 0, delta });
		if (mDisplay.keyboard.isPressed('A'))
			mCamera.translate({ -delta, 0, 0 });
		if (mDisplay.keyboard.isPressed('S'))
			mCamera.translate({ 0, 0, -delta });
		if (mDisplay.keyboard.isPressed('D'))
			mCamera.translate({ delta, 0, 0 });
		if (mDisplay.keyboard.isPressed(VK_SPACE))
			mCamera.translate({ 0, delta, 0 });
		if (mDisplay.keyboard.isPressed('C'))
			mCamera.translate({ 0, -delta, 0 });
	}

	if(!mDisplay.isCursorEnabled())
	{
		while (const auto d = mDisplay.mouse.readRawDelta())
		{
			mCamera.rotate(d->x, d->y);
		}
	}


	calculateFrameStatistics(mPerformanceTimer);

	mCamera.spawnControlWindow();
	mLight.spawnControlWindow();
	mNano.showImguiWindow("Nanosuit");

	mDisplay.getGraphics().swap();
}

