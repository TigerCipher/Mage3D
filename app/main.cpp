// Mage3D
// Copyright (C) 2020 Blue Moon Development
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// 
// Contact: team@bluemoondev.org
// 
// File Name: main.cpp
// Date File Created: 07/11/2020 at 4:19 PM
// Author: Matt / TigerCipher


#include <string>
#include <mage/mage.h>
#include <glm/gtx/transform.hpp>

int main(int argc, char** argv)
{
	//std::vector<mage::Vertex> verts = {
	//		mage::Vertex(-0.5f, -0.5f, 0.5f, 0, 1, 0), //v0
	//		mage::Vertex(0.5f, -0.5f, 0.5f, 0, 0, 1), //v1
	//		mage::Vertex(0.5f, -0.5f, -0.5f, 0, 1, 1), //v2
	//		mage::Vertex(-0.5f, -0.5f, -0.5f, 1, 0, 1), //v3
	//		mage::Vertex(0, 0.5f, 0, 1, 0, 0), //v4
	//};
	//
	//std::vector<uint> ints = {
	//		0, 4, 1,
	//		1, 4, 2,
	//		2, 4, 3,
	//		3, 4, 0,
	//		0, 3, 2,
	//		2, 1, 0
	//};

	//TODO: Texture coords don't seem to map properly when using indices
	list<mage::Vertex> verts = {
			mage::Vertex(-0.5f, -0.5f, -0.5f, 0, 0, 1, 0, 0),
			mage::Vertex(0.5f, -0.5f, -0.5f, 1, 0, 0, 1, 0),
			mage::Vertex(0.5f, 0.5f, -0.5f, 1, 1, 0, 0, 1),
			mage::Vertex(0.5f, 0.5f, -0.5f, 1, 1, 0, 0, 1),
			mage::Vertex(-0.5f, 0.5f, -0.5f, 0, 1, 1, 1, 1),
			mage::Vertex(-0.5f, -0.5f, -0.5f, 0, 0, 1, 0, 0),

			mage::Vertex(-0.5f, -0.5f, 0.5f, 0, 0, 1, 0, 0),
			mage::Vertex(0.5f, -0.5f, 0.5f, 1, 0, 0, 1, 0),
			mage::Vertex(0.5f, 0.5f, 0.5f, 1, 1, 0, 0, 1),
			mage::Vertex(0.5f, 0.5f, 0.5f, 1, 1, 0, 0, 1),
			mage::Vertex(-0.5f, 0.5f, 0.5f, 0, 1, 1, 1, 1),
			mage::Vertex(-0.5f, -0.5f, 0.5f, 0, 0, 1, 0, 0),

			mage::Vertex(-0.5f, 0.5f, 0.5f, 1, 0, 1, 0, 0),
			mage::Vertex(-0.5f, 0.5f, -0.5f, 1, 1, 0, 1, 0),
			mage::Vertex(-0.5f, -0.5f, -0.5f, 0, 1, 0, 0, 1),
			mage::Vertex(-0.5f, -0.5f, -0.5f, 0, 1, 0, 0, 1),
			mage::Vertex(-0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 1),
			mage::Vertex(-0.5f, 0.5f, 0.5f, 1, 0, 1, 0, 0),

			mage::Vertex(0.5f, 0.5f, 0.5f, 1, 0, 1, 0, 0),
			mage::Vertex(0.5f, 0.5f, -0.5f, 1, 1, 0, 1, 0),
			mage::Vertex(0.5f, -0.5f, -0.5f, 0, 1, 0, 0, 1),
			mage::Vertex(0.5f, -0.5f, -0.5f, 0, 1, 0, 0, 1),
			mage::Vertex(0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 1),
			mage::Vertex(0.5f, 0.5f, 0.5f, 1, 0, 1, 0, 0),

			mage::Vertex(-0.5f, -0.5f, -0.5f, 0, 1, 1, 0, 0),
			mage::Vertex(0.5f, -0.5f, -0.5f, 1, 1, 0, 1, 0),
			mage::Vertex(0.5f, -0.5f, 0.5f, 1, 0, 0, 0, 1),
			mage::Vertex(0.5f, -0.5f, 0.5f, 1, 0, 0, 0, 1),
			mage::Vertex(-0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 1),
			mage::Vertex(-0.5f, -0.5f, -0.5f, 0, 1, 1, 0, 0),

			mage::Vertex(-0.5f, 0.5f, -0.5f, 0, 1, 1, 0, 0),
			mage::Vertex(0.5f, 0.5f, -0.5f, 1, 1, 0, 1, 0),
			mage::Vertex(0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 1),
			mage::Vertex(0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 1),
			mage::Vertex(-0.5f, 0.5f, 0.5f, 0, 0, 1, 1, 1),
			mage::Vertex(-0.5f, 0.5f, -0.5f, 0, 1, 1, 0, 0),
	};

	list<uint> ints = {
			0, 1, 2,
			3, 4, 5,

			6, 7, 8,
			9, 10, 11,

			12, 13, 14,
			15, 16, 17,

			18, 19, 20,
			21, 22, 23,

			24, 25, 26,
			27, 28, 29,

			30, 31, 32,
			33, 34, 35
	};

	// UV coords can't be shared between vertices, so textures don't work properly
	list<mage::Vertex> testVerts = {
			mage::Vertex(-0.5f, -0.5f, -0.5f, 0, 0),
			mage::Vertex(0.5f, -0.5f, -0.5f, 0, 1),
			mage::Vertex(0.5f, -0.5f, 0.5f, 1, 1),
			mage::Vertex(-0.5f, -0.5f, 0.5f, 1, 0),

			mage::Vertex(-0.5f, 0.5f, -0.5f, 0, 0),
			mage::Vertex(0.5f, 0.5f, -0.5f, 0, 1),
			mage::Vertex(0.5f, 0.5f, 0.5f, 1, 1),
			mage::Vertex(-0.5f, 0.5f, 0.5f, 1, 0),
	};

	list<uint> testInts = {
			0, 1, 2,
			2, 0, 3,

			3, 7, 4,
			4, 3, 0,

			0, 4, 5,
			5, 0, 1,

			1, 5, 6,
			6, 1, 2,

			2, 6, 7,
			7, 2, 3,

			4, 7, 6,
			6, 4, 5
	};

	list<mage::Vertex> planeVerts = {
			mage::Vertex(-2, -2, -2, 0, 0),
			mage::Vertex(2, -2, -2, 0, 1),
			mage::Vertex(2, -2, 2, 1, 1),
			mage::Vertex(-2, -2, 2, 1, 0),
	};

	list<uint> planeInts = {
			0, 1, 2,
			2, 0, 3
	};

	mage::clearConsole(mage::console::WHITE);
	mage::Input input;
	const mage::Display display("Mage3D Testing", 1920, 1080, &input);
	mage::Camera camera(&display, glm::vec3(0, 0, 5));
	//auto* testMesh = new mage::Mesh(vertices, 5, indices, 18);
	auto* testMesh = new mage::Mesh(verts, ints);
	auto* planeMesh = new mage::Mesh(planeVerts, planeInts);
	mage::Renderer renderer;

	mage::Shader shader("./assets/basic_color");
	mage::Texture texture("./assets/default.png");


	mage::Timer timer;
	int frames = 0;
	mage::Timer rotTimer;
	while (!display.isClosed())
	{
		timer.update();

		if (input.keyPressed(GLFW_KEY_ESCAPE))
			display.toggleCursor();

		camera.update(input, timer.delta());
		display.clear(0, 0, 0);
		// Render
		shader.enable();
		texture.enable();
		shader.setUniform1i("tex", texture.getId());
		glm::mat4 rot(1);
		rot = glm::rotate((float) rotTimer.elapsed() * 0.6f, glm::vec3(1, 1, 1));
		shader.setUniformMatf("model", rot);
		shader.setUniformMatf("projection", camera.getProjectionMatrix());
		shader.setUniformMatf("view", camera.getViewMatrix());

		renderer.draw(testMesh);

		texture.disable();
		shader.disable();

		shader.enable();
		texture.enable();
		shader.setUniform1i("tex", texture.getId());
		glm::mat4 plane(1);
		shader.setUniformMatf("model", plane);
		shader.setUniformMatf("projection", camera.getProjectionMatrix());
		shader.setUniformMatf("view", camera.getViewMatrix());

		renderer.draw(planeMesh);

		texture.disable();
		shader.disable();

		//if(input.keyDown(GLFW_KEY_C))
		//	mage::println(mage::console::BRIGHT_BLUE, "C key is down!");
		//if(input.keyPressed(GLFW_KEY_C))
		//	mage::println(mage::console::BRIGHT_BLUE, "C key was pressed!");
		//if(input.keyReleased(GLFW_KEY_C))
		//	mage::println(mage::console::BRIGHT_BLUE, "C key was released!");
		if (input.mouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			mage::println(mage::console::BRIGHT_BLUE, "Mouse pos: ({}, {})", input.getMouseX(),
						  input.getMouseY());
		}

		input.update();
		display.update();
		double elapsed = timer.elapsed();
		// Calculating fps every single frame is a bit pointless and performance damaging
		//mage::println(mage::console::BRIGHT_RED, "FPS: {}, frames: {}, time: {}", fps, frames, elapsed);
		//std::string newTitle = fmt::format("{}{}", "Mage3D Testing -- FPS: ", fps);
		//const char* newTitleCstr = newTitle.c_str();
		//display.setTitle(newTitleCstr);
		if (elapsed >= 1.0)
		{
			double fps = frames / elapsed;
			mage::println(mage::console::BRIGHT_YELLOW, "Average FPS: {} | Precise: {}", frames, fps);
			std::string newTitle = fmt::format("{}{:.{}f}", "Mage3D Testing -- FPS: ", fps, 4);
			const char* newTitleCstr = newTitle.c_str();
			display.setTitle(newTitleCstr);
			frames = 0;
			timer.reset();
		} else frames++;

	}

	delete testMesh;
	return 0;
}
