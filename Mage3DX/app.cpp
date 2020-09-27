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
 * File Name: app.cpp
 * Date File Created: 9/25/2020 at 11:49 PM
 * Author: Matt
 */
#include "app.h"
#include "box.h"
#include "melon.h"
#include "pyramid.h"
#include "sheet.h"
#include "texture_surface.h"


mage::App::App(int width, int height, const char* title) :
	m_display(width, height, title),
	m_running(true),
	m_light(m_display.getGraphics())
{
	GDIPlusManager::start();
	class Factory
	{
	public:
		explicit Factory(Graphics& gfx) :
			gfx(gfx)
		{ }

		std::unique_ptr<IRenderable> operator()()
		{
			return createScope<Box>(gfx, rng, adist, ddist, odist, rdist, bdist);
		}

	private:
		Graphics& gfx;
		std::mt19937 rng { std::random_device { }() };
		std::uniform_real_distribution<float> adist { 0.0f, PI * 2.0f };
		std::uniform_real_distribution<float> ddist { 0.0f, PI * 0.5f };
		std::uniform_real_distribution<float> odist { 0.0f, PI * 0.08f };
		std::uniform_real_distribution<float> rdist { 6.0f, 20.0f };
		std::uniform_real_distribution<float> bdist { 0.4f, 3.0f };
		std::uniform_int_distribution<int> latdist { 5, 20 };
		std::uniform_int_distribution<int> longdist { 10, 40 };
		std::uniform_int_distribution<int> typedist { 0, 4 };
	};

	Factory f(m_display.getGraphics());
	m_renderables.reserve(NUM_RENDERS);
	std::generate_n(std::back_inserter(m_renderables), NUM_RENDERS, f);


	m_display.getGraphics().setProjection(dx::XMMatrixPerspectiveLH(1.0f,
										  m_display.getAspectRatio(),
										  0.5f, 40.0f));

}

mage::App::~App()
{
	GDIPlusManager::stop();
}

int mage::App::run()
{
	LOG_INFO("Main loop beginning");
	while (m_running)
	{
		if (const auto exitCode = Display::processMessages())
			return *exitCode;
		runFrame();
	}
	ImguiManager::stop();
	return 0;
}

static void calculateFrameStatistics(mage::Timer& timer)
{
	static float fps = 0;
	static float avgFps = 0;
	static int frameCount = 0;
	static float prntAvgFps = 0;
	static int prntFrameCount = 0;

	if (mage::ImguiManager::isEnabled())
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
				  ImGui::Text("Frame Stats: %.3f ms/frame (%.1f FPS)", 1000.0f / fps, fps),
				  ImGui::Text("Average Frame Stats: %.3f ms/frame (%.1f FPS)", 1000.0f / prntAvgFps, prntAvgFps),
				  ImGui::TextColored(ImVec4(1, 1, 0, 1), "Processed %i frames", prntFrameCount)
	);
}

void mage::App::runFrame()
{
	float delta = m_timer.markPoint() * globalSpeed;

	if (m_display.m_keyboard.isPressedOnce(VK_NUMPAD5)) ImguiManager::toggle();

	m_display.getGraphics().clear(0.07f, 0, 0.12f);
	m_display.getGraphics().setCamera(m_camera.getViewMatrix());
	m_light.bind(m_display.getGraphics());

	for (auto& b : m_renderables)
	{
		b->update(m_display.m_keyboard.isPressed(VK_SPACE) ? 0.0f : delta);
		b->render(m_display.getGraphics());
	}

	m_light.render(m_display.getGraphics());
	//if (showDemo) { ImGui::ShowDemoWindow(&showDemo); }

	IMGUI_WRAP("Global Simulation Speed",
			   ImGui::SliderFloat("Speed Factor", &globalSpeed, 0.0f, 5.0f));
	calculateFrameStatistics(m_performanceTimer);

	m_camera.spawnControlWindow();
	m_light.spawnControlWindow();

	m_display.getGraphics().swap();
}
