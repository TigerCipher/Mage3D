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

	mage::Vertex data[] = {
			mage::Vertex(0, 0.5f, 0, 1, 0, 0), //v0
			mage::Vertex(-0.5f, -0.5f, 0.5f, 0, 1, 0), //v1
			mage::Vertex(0.5f, -0.5f, 0.5f, 0, 0, 1), //v2

			mage::Vertex(0, 0.5f, 0, 1, 0, 0), //v0
			mage::Vertex(0.5f, -0.5f, 0.5f, 0, 0, 1), //v2
			mage::Vertex(0.5f, -0.5f, -0.5f, 0, 1, 0), //v3

			mage::Vertex(0, 0.5f, 0, 1, 0, 0), //v0
			mage::Vertex(0.5f, -0.5f, -0.5f, 0, 1, 0), //v3
			mage::Vertex(-0.5f, -0.5f, -0.5f, 0, 0, 1), //v4

			mage::Vertex(0, 0.5f, 0, 1, 0, 0), //v0
			mage::Vertex(-0.5f, -0.5f, -0.5f, 0, 0, 1), //v4
			mage::Vertex(-0.5f, -0.5f, 0.5f, 0, 1, 0), //v1
	};
	mage::clearConsole(mage::console::WHITE);
	mage::Input input;
	const mage::Display display("Mage3D Testing", 1920, 1080, &input);
	auto* testMesh = new mage::Mesh(data);
	mage::Renderer renderer;

	auto* shader = new mage::Shader("./assets/basic");


	mage::Timer timer;
	int frames = 0;
	mage::Timer rotTimer;
	while (!display.isClosed())
	{
		display.clear(0, 0, 0);
		// Render
		shader->enable();
		glm::mat4 rot(1);
		rot = glm::rotate((float) rotTimer.elapsed() * 0.5f, glm::vec3(0, 1, 0));
		shader->setUniformMatf("rotation", rot);

		renderer.draw(testMesh);


		shader->disable();

		//if(input.keyDown(GLFW_KEY_C))
		//	mage::println(mage::console::BRIGHT_BLUE, "C key is down!");
		//if(input.keyPressed(GLFW_KEY_C))
		//	mage::println(mage::console::BRIGHT_BLUE, "C key was pressed!");
		//if(input.keyReleased(GLFW_KEY_C))
		//	mage::println(mage::console::BRIGHT_BLUE, "C key was released!");
		if(input.mouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			mage::println(mage::console::BRIGHT_BLUE, "Mouse pos: ({}, {})", input.getMouseX(), input.getMouseY());
		}

		input.update();
		display.update();

		if (timer.elapsed() >= 1.0)
		{
			// printf("FPS: %i\n", frames);
			mage::println(mage::console::BRIGHT_RED, "FPS: {}", frames);
			// fmt::print(fg(fmt::terminal_color::bright_cyan), "FPS: {}\n", frames);
			frames = 0;
			timer.reset();
		} else frames++;
	}

	delete testMesh;
	delete shader;
	return 0;
}
