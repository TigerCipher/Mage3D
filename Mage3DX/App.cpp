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
#include "Box.h"
#include "TextureSurface.h"
#include "ImguiManager.h"


App::App(const int width, const int height, const char* title) :
	mDisplay(width, height, title),
	mLight(mDisplay.getGraphics()),
	mRunning(true),
	mNano(mDisplay.getGraphics(), "assets\\models\\nanosuit.gltf")
{
	GDIPlusManager::start();

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
	GDIPlusManager::stop();
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
		static bool metrics = true;
		ImGui::ShowMetricsWindow(&metrics);
		frameCount++;
		fps = ImGui::GetIO().Framerate;
		avgFps += fps;
		if (timer.peek() >= 1.0f)
		{
			prntAvgFps = avgFps / (float) frameCount;
			avgFps = 0;
			prntFrameCount = frameCount;
			frameCount = 0;
			timer.markPoint();
		}
	}

	IMGUI_WRAP_RD("Performance Statistics",
		ImGui::Text("Frame Stats: %.3f ms/frame (%.1f FPS)",         1000.0f / fps,        fps),
		ImGui::Text("Average Frame Stats: %.3f ms/frame (%.1f FPS)", 1000.0f / prntAvgFps, prntAvgFps),
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Processed %i frames", prntFrameCount)
		);
}


void App::runFrame()
{
	const auto delta = mTimer.markPoint() * mGlobalSpeed;

	//if (mDisplay.keyboard.isPressedOnce(VK_NUMPAD5)) ImguiManager::toggle();

	mDisplay.getGraphics().clear(0.07f, 0, 0.12f);
	mDisplay.getGraphics().setCamera(mCamera.getViewMatrix());
	mLight.bind(mDisplay.getGraphics(), mCamera.getViewMatrix());


	mNano.render(mDisplay.getGraphics());
	mLight.render(mDisplay.getGraphics());

	if(mDisplay.keyboard.isPressedOnce(VK_INSERT))
	{
		mDisplay.toggleCursor();
		mDisplay.mouse.toggleRawInput();
	}

	calculateFrameStatistics(mPerformanceTimer);

	mCamera.spawnControlWindow();
	mLight.spawnControlWindow();
	mNano.showImguiWindow("Nanosuit");
	showRawInput();

	mDisplay.getGraphics().swap();
}

void App::showRawInput()
{
	while(const auto d = mDisplay.mouse.readRawDelta())
	{
		x += d->x;
		y += d->y;
	}

	IMGUI_WRAP("Raw Input",
		ImGui::Text("Data: (%d, %d)", x, y));
}

