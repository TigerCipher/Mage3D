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
#include "ImguiManager.h"


GDIPlusManager gGdip;

App::App(const int width, const int height, const char* title) :
	mDisplay(width, height, title),
	mLight(mDisplay.getGraphics()),
	mRunning(true),
	mPlane(mDisplay.getGraphics()),
	mWall(mDisplay.getGraphics(), "assets\\models\\brickwall.obj")
	//mNano(mDisplay.getGraphics(), "assets\\models\\nanosuit.obj"),
	//mNano2(mDisplay.getGraphics(), "assets\\models\\nanosuit.obj"),
	//mCube(mDisplay.getGraphics(), 4.0f)
{
	mDisplay.getGraphics().setProjection(dx::XMMatrixPerspectiveLH(1.0f,
		mDisplay.getAspectRatio(),
		0.5f, 40.0f));
	mDisplay.toggleCursor(0);

	mDisplay.getGraphics().addFont("Courier New", "assets\\fonts\\courier_new.sf");
	mDisplay.getGraphics().addFont("Agency FB", "assets\\fonts\\agencyfb.sf");
	mDisplay.getGraphics().addFont("Arial", "assets\\fonts\\arial.sf");
	mDisplay.getGraphics().addFont("Impact", "assets\\fonts\\impact.sf");
	mDisplay.getGraphics().addFont("Ink Free", "assets\\fonts\\inkfree.sf");
	mDisplay.getGraphics().addFont("Kristen ITC", "assets\\fonts\\kristen_itc.sf");
	mDisplay.getGraphics().addFont("OCR", "assets\\fonts\\ocr.sf");

	mPlane.setPosition({ 4.0f, 0, 0 });
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

void calculate_frame_statistics(Timer& timer, float* retFps)
{
	static float fps = 0;
	static float avgFps = 0;
	static int frameCount = 0;
	static float prntAvgFps = 0;
	static int prntFrameCount = 0;

	IMGUI_BEGIN("Performance Statistics", nullptr, ImGuiWindowFlags_AlwaysAutoResize)
	{
		frameCount++;
		fps = IMGUI_FUNC_R(GetIO().Framerate, frameCount);
		avgFps += fps;
		if (timer.peek() >= 1.0f)
		{
			prntAvgFps = avgFps / static_cast<float>(frameCount);
			*retFps = prntAvgFps;
			avgFps = 0;
			prntFrameCount = frameCount;
			frameCount = 0;
			timer.markPoint();
		}


		IMGUI_FUNC(Text("Frame Stats: %.3f ms/frame (%.1f FPS)", 1000.0f / fps, fps));
		IMGUI_FUNC(Text("Average Frame Stats: %.3f ms/frame (%.1f FPS)", 1000.0f / prntAvgFps, prntAvgFps));
		IMGUI_FUNC(TextColored(ImVec4(1, 1, 0, 1), "Processed %i frames", prntFrameCount));
	}
	IMGUI_END
}


void App::runFrame()
{
	const auto delta = mTimer.markPoint() * mGlobalSpeed;

	if (mDisplay.keyboard.isPressedOnce(VK_NUMPAD5)) ImguiManager::toggle();

	mDisplay.getGraphics().clear(0.07f, 0, 0.12f);
	mDisplay.getGraphics().setCamera(mCamera.getViewMatrix());
	mLight.bind(mDisplay.getGraphics(), mCamera.getViewMatrix());


	mWall.render(mDisplay.getGraphics());
	mPlane.render(mDisplay.getGraphics());

	mLight.render(mDisplay.getGraphics());

	if (mDisplay.keyboard.isPressedOnce(VK_ESCAPE))
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
			mCamera.rotate(static_cast<float>(d->x), static_cast<float>(d->y));
		}
	}

	static float fps = 0;
	calculate_frame_statistics(mPerformanceTimer, &fps);

	mCamera.spawnControlWindow();
	mLight.spawnControlWindow();
	mWall.showImguiWindow("Brickwall");
	mPlane.spawnControlWindow(mDisplay.getGraphics());

	mDisplay.getGraphics().drawText("OCR", fmt::format("FPS: {:.2f}", fps), 5, 5);

	mDisplay.getGraphics().swap();
}

