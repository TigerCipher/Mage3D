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
#include "ImguiManager.h"
#include "TextureProcessor.h"
#include "Settings.h"

#include "Util.h"

#include "DynamicConstantBuffer.h"



App::App(const int width, const int height, const char* title, const std::string& cmdLine) :
	mCommandLine(cmdLine),
	mDisplay(width, height, title, false),
	mRunning(true),
	mScriptCommander(tokenize_quoted(cmdLine)),
	mLight(mDisplay.getGraphics()),
	mSponza(mDisplay.getGraphics(), "assets\\models\\sponza.obj", 1.0f / 20.0f),
	mBluePlane(mDisplay.getGraphics(), 6.0f, {0.2f, 0.2f, 1.0f, 0}),
	mRedPlane(mDisplay.getGraphics(), 6.0f, {1.0f, 0.2f, 0.2f, 0})
{


	dcb::Layout s;
	s.add<dcb::Struct>("test");
	s["test"].add<dcb::Float3>("idk");
	s["test"].add<dcb::Float>("wut");
	
	s.add<dcb::Array>("arr");
	s["arr"].set<dcb::Struct>(4);
	s["arr"].type().add<dcb::Float>("flour");
	s["arr"].type().add<dcb::Array>("monkey");
	s["arr"].type()["monkey"].set<dcb::Float>(6);
	s["arr"].type().add<dcb::Array>("meta");
	s["arr"].type()["meta"].set<dcb::Array>(6);
	s["arr"].type()["meta"].type().set<dcb::Matrix>(4);
	s.add<dcb::Array>("bork");
	s["bork"].set<dcb::Struct>(2);
	s["bork"].type().add<dcb::Float>("meow");
	s["bork"].type().add<dcb::Float>("moo");

	auto b = dcb::Buffer::build(s);
	b["test"]["idk"] = vec3f(0, 1, 0);
	b["test"]["wut"] = 34.25f;
	b["arr"][2]["monkey"][5] = 12.5f;

	storeMatrix4(&b["arr"][2]["meta"][5][3], identityMatrix());
	
	vec3f v = b["test"]["idk"];
	float w = b["test"]["wut"];
	b["bork"][0]["meow"] = 2.45f;
	b["bork"][0]["moo"] = 4532.2344f;
	b["bork"][1]["meow"] = 2.2f;
	b["bork"][1]["moo"] = 234.00234f;
	float fg = b["arr"][2]["monkey"][5];
	float fdsg = b["bork"][0]["meow"];
	float srefd = b["bork"][1]["moo"];
	mat4x4 metaMagic = b["arr"][2]["meta"][5][3];

	const auto& test = b;
	mat4x4 blorpgh = test["arr"][2]["meta"][5][3];

	//test["arr"][2]["meta"][5][3] = identityMatrix(); // doesn't compile (meaning my const correctness is working properly)

	mDisplay.getGraphics().setProjection(dx::XMMatrixPerspectiveLH(1.0f,
		mDisplay.getAspectRatio(),
		0.5f, 400.0f));
	mDisplay.toggleCursor(0);

	//mSponza.load(mDisplay.getGraphics(), "assets\\models\\sponza.obj", 1.0f / 20.0f);

	mDisplay.getGraphics().addFont("Courier New", "assets\\fonts\\courier_new.sf");
	mDisplay.getGraphics().addFont("Agency FB", "assets\\fonts\\agencyfb.sf");
	mDisplay.getGraphics().addFont("Arial", "assets\\fonts\\arial.sf");
	mDisplay.getGraphics().addFont("Impact", "assets\\fonts\\impact.sf");
	mDisplay.getGraphics().addFont("Ink Free", "assets\\fonts\\inkfree.sf");
	mDisplay.getGraphics().addFont("Kristen ITC", "assets\\fonts\\kristen_itc.sf");
	mDisplay.getGraphics().addFont("OCR", "assets\\fonts\\ocr.sf");

	mBluePlane.setPosition(mCamera.getPosition());
	mRedPlane.setPosition(mCamera.getPosition());

	mDisplay.show();
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

void calculate_frame_statistics(Timer& timer, float fps, Graphics& gfx)
{
	static float avgFps = 0;
	static int frameCount = 0;
	static float prntAvgFps = 1;
	static int prntFrameCount = 0;

	frameCount++;
	avgFps += fps;
	if (timer.peek() >= 1.0f)
	{
		prntAvgFps = avgFps / static_cast<float>(frameCount);
		avgFps = 0;
		prntFrameCount = frameCount;
		frameCount = 0;
		timer.markPoint();
		fmt::print("FPS: {:.2f}    -    {:.3f} ms/frame\n", prntAvgFps, 1000.0f / prntAvgFps);
	}
	
	// TODO: Text rendering performance needs to be improved
	// Drawing text seems to be a bit of a performance impact
	// Initial testing suggests an average of a 30-40 FPS loss
	// Currently drawing text starts and ends a spritebatch each call. Might be best to create a UI/text layer and have all
	// text draw calls queued up to be rendered in the same spritebatch load?
	//gfx.drawText("OCR", fmt::format("FPS: {:.2f}", prntAvgFps), 5, 5, dx::Colors::Red);
	//gfx.drawText("OCR", fmt::format("{:.3f} ms/frame", 1000.0f / prntAvgFps), 5, 45);
}


void App::runFrame()
{
	float fps = 1.0f / mTimer.peek();
	const auto delta = mTimer.markPoint() * mGlobalSpeed;

	if (mDisplay.keyboard.isPressedOnce(VK_NUMPAD5)) ImguiManager::toggle();

	mDisplay.getGraphics().clear(0.07f, 0, 0.12f);
	mDisplay.getGraphics().setCamera(mCamera.getViewMatrix());
	mLight.bind(mDisplay.getGraphics(), mCamera.getViewMatrix());
	

	mLight.render(mDisplay.getGraphics());

	mSponza.render(mDisplay.getGraphics());

	mRedPlane.render(mDisplay.getGraphics());
	mBluePlane.render(mDisplay.getGraphics());

	if (mDisplay.keyboard.isPressedOnce(VK_ESCAPE))
	{
		mDisplay.toggleCursor();
		mDisplay.mouse.toggleRawInput();
	}

	if(!mDisplay.isCursorEnabled())
	{
		static bool changed = false;
		if(mDisplay.keyboard.isPressed(VK_SHIFT) && !changed)
		{
			mCamera.increaseSpeed(4.0f);
			changed = true;
		}else if(!mDisplay.keyboard.isPressed(VK_SHIFT) && changed)
		{
			changed = false;
			mCamera.decreaseSpeed(4.0f);
		}
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


	mLight.spawnControlWindow();

	mSponza.showImguiWindow(mDisplay.getGraphics(), "Crytek Sponza");
	mBluePlane.spawnControlWindow(mDisplay.getGraphics(), "Blue Plane");
	mRedPlane.spawnControlWindow(mDisplay.getGraphics(), "Red Plane");

	//mDisplay.getGraphics().drawText("Kristen ITC", "Hello!", 400, 400,
	//	DirectX::Colors::OrangeRed, 2.0f, 45);
	//show_debug_console();

	calculate_frame_statistics(mPerformanceTimer, fps, mDisplay.getGraphics());
	mDisplay.getGraphics().swap();
}


