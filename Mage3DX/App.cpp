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


App::App(int width, int height, const char* title) :
	m_display(width, height, title),
	m_light(m_display.getGraphics()),
	m_running(true),
	m_nano(m_display.getGraphics(), "assets\\models\\nanosuit.obj")
{
	GDIPlusManager::start();

	m_display.getGraphics().setProjection(dx::XMMatrixPerspectiveLH(1.0f,
		m_display.getAspectRatio(),
		0.5f, 40.0f));
}

App::~App()
{
	if (m_running)
		stop();
}

int App::run()
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

void App::stop()
{
	m_running = false;
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
	const auto delta = m_timer.markPoint() * m_globalSpeed;

	if (m_display.keyboard.isPressedOnce(VK_NUMPAD5)) ImguiManager::toggle();

	m_display.getGraphics().clear(0.07f, 0, 0.12f);
	m_display.getGraphics().setCamera(m_camera.getViewMatrix());
	m_light.bind(m_display.getGraphics(), m_camera.getViewMatrix());


	const auto transform = dx::XMMatrixRotationRollPitchYaw(m_nanoPos.roll, m_nanoPos.pitch, m_nanoPos.yaw)
	    * dx::XMMatrixTranslation(m_nanoPos.x, m_nanoPos.y, m_nanoPos.z);
	m_nano.render(m_display.getGraphics(), transform);
	m_light.render(m_display.getGraphics());

	calculateFrameStatistics(m_performanceTimer);

	m_camera.spawnControlWindow();
	m_light.spawnControlWindow();
	showNanoControl();

	m_display.getGraphics().swap();
}

void App::showNanoControl()
{
	IMGUI_WRAP("Nanosuit Model",
		ImGui::Text("Orientation"),
		ImGui::SliderAngle("Roll", &m_nanoPos.roll, -180.0f, 180.0f),
		ImGui::SliderAngle("Pitch", &m_nanoPos.pitch, -180.0f, 180.0f),
		ImGui::SliderAngle("Yaw", &m_nanoPos.yaw, -180.0f, 180.0f),

		ImGui::Text("Position"),
		ImGui::SliderFloat("X", &m_nanoPos.x, -20.0f, 20.0f),
		ImGui::SliderFloat("Y", &m_nanoPos.y, -20.0f, 20.0f),
		ImGui::SliderFloat("Z", &m_nanoPos.z, -20.0f, 20.0f)

		);
}
