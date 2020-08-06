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
#include <iostream>

int main(int argc, char** argv)
{

	// UV coords can't be shared between vertices, so textures don't work properly
	list<mage::Vertex> testVerts = {
			mage::Vertex(mage::Position(-0.1f, -0.1f, -1.1f)),
			mage::Vertex(mage::Position(0.1f, -0.1f, -1.1f)),
			mage::Vertex(mage::Position(0.1f, -0.1f, -0.9f)),
			mage::Vertex(mage::Position(-0.1f, -0.1f, -0.9f)),

			mage::Vertex(mage::Position(-0.1f, 0.1f, -1.1f)),
			mage::Vertex(mage::Position(0.1f, 0.1f, -1.1f)),
			mage::Vertex(mage::Position(0.1f, 0.1f, -0.9f)),
			mage::Vertex(mage::Position(-0.1f, 0.1f, -0.9f)),
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
			mage::Vertex(mage::Position(-2, -2, -2), mage::Normal(0, -0.5f, 0), mage::TexCoords(0, 0)),
			mage::Vertex(mage::Position(2, -2, -2), mage::Normal(0, -0.5f, 0), mage::TexCoords(0, 1)),
			mage::Vertex(mage::Position(2, -2, 2), mage::Normal(0, -0.5f, 0), mage::TexCoords(1, 1)),
			mage::Vertex(mage::Position(-2, -2, 2), mage::Normal(0, -0.5f, 0), mage::TexCoords(1, 0)),
	};

	list<uint> planeInts = {
			0, 1, 2,
			2, 0, 3
	};

	list<mage::Vertex> verts = {
			mage::Vertex(mage::Position(-0.5f, -0.5f, -0.5f), mage::Normal(0, 0, -1), mage::TexCoords(0, 0)),
			mage::Vertex(mage::Position(0.5f, -0.5f, -0.5f), mage::Normal(0, 0, -1), mage::TexCoords(1, 0)),
			mage::Vertex(mage::Position(0.5f, 0.5f, -0.5f), mage::Normal(0, 0, -1), mage::TexCoords(1, 1)),
			mage::Vertex(mage::Position(0.5f, 0.5f, -0.5f), mage::Normal(0, 0, -1), mage::TexCoords(1, 1)),
			mage::Vertex(mage::Position(-0.5f, 0.5f, -0.5f), mage::Normal(0, 0, -1), mage::TexCoords(0, 1)),
			mage::Vertex(mage::Position(-0.5f, -0.5f, -0.5f), mage::Normal(0, 0, -1), mage::TexCoords(0, 0)),

			mage::Vertex(mage::Position(-0.5f, -0.5f, 0.5f), mage::Normal(0, 0, 1), mage::TexCoords(0, 0)),
			mage::Vertex(mage::Position(0.5f, -0.5f, 0.5f), mage::Normal(0, 0, 1), mage::TexCoords(1, 0)),
			mage::Vertex(mage::Position(0.5f, 0.5f, 0.5f), mage::Normal(0, 0, 1), mage::TexCoords(1, 1)),
			mage::Vertex(mage::Position(0.5f, 0.5f, 0.5f), mage::Normal(0, 0, 1), mage::TexCoords(1, 1)),
			mage::Vertex(mage::Position(-0.5f, 0.5f, 0.5f), mage::Normal(0, 0, 1), mage::TexCoords(0, 1)),
			mage::Vertex(mage::Position(-0.5f, -0.5f, 0.5f), mage::Normal(0, 0, 1), mage::TexCoords(0, 0)),

			mage::Vertex(mage::Position(-0.5f, 0.5f, 0.5f), mage::Normal(-1, 0, 0), mage::TexCoords(1, 0)),
			mage::Vertex(mage::Position(-0.5f, 0.5f, -0.5f), mage::Normal(-1, 0, 0), mage::TexCoords(1, 1)),
			mage::Vertex(mage::Position(-0.5f, -0.5f, -0.5f), mage::Normal(-1, 0, 0), mage::TexCoords(0, 1)),
			mage::Vertex(mage::Position(-0.5f, -0.5f, -0.5f), mage::Normal(-1, 0, 0), mage::TexCoords(0, 1)),
			mage::Vertex(mage::Position(-0.5f, -0.5f, 0.5f), mage::Normal(-1, 0, 0), mage::TexCoords(0, 0)),
			mage::Vertex(mage::Position(-0.5f, 0.5f, 0.5f), mage::Normal(-1, 0, 0), mage::TexCoords(1, 0)),

			mage::Vertex(mage::Position(0.5f, 0.5f, 0.5f), mage::Normal(1, 0, 0), mage::TexCoords(1, 0)),
			mage::Vertex(mage::Position(0.5f, 0.5f, -0.5f), mage::Normal(1, 0, 0), mage::TexCoords(1, 1)),
			mage::Vertex(mage::Position(0.5f, -0.5f, -0.5f), mage::Normal(1, 0, 0), mage::TexCoords(0, 1)),
			mage::Vertex(mage::Position(0.5f, -0.5f, -0.5f), mage::Normal(1, 0, 0), mage::TexCoords(0, 1)),
			mage::Vertex(mage::Position(0.5f, -0.5f, 0.5f), mage::Normal(1, 0, 0), mage::TexCoords(0, 0)),
			mage::Vertex(mage::Position(0.5f, 0.5f, 0.5f), mage::Normal(1, 0, 0), mage::TexCoords(1, 0)),

			mage::Vertex(mage::Position(-0.5f, -0.5f, -0.5f), mage::Normal(0, -1, 0), mage::TexCoords(0, 1)),
			mage::Vertex(mage::Position(0.5f, -0.5f, -0.5f), mage::Normal(0, -1, 0), mage::TexCoords(1, 1)),
			mage::Vertex(mage::Position(0.5f, -0.5f, 0.5f), mage::Normal(0, -1, 0), mage::TexCoords(1, 0)),
			mage::Vertex(mage::Position(0.5f, -0.5f, 0.5f), mage::Normal(0, -1, 0), mage::TexCoords(1, 0)),
			mage::Vertex(mage::Position(-0.5f, -0.5f, 0.5f), mage::Normal(0, -1, 0), mage::TexCoords(0, 0)),
			mage::Vertex(mage::Position(-0.5f, -0.5f, -0.5f), mage::Normal(0, -1, 0), mage::TexCoords(0, 1)),

			mage::Vertex(mage::Position(-0.5f, 0.5f, -0.5f), mage::Normal(0, 1, 0), mage::TexCoords(0, 1)),
			mage::Vertex(mage::Position(0.5f, 0.5f, -0.5f), mage::Normal(0, 1, 0), mage::TexCoords(1, 1)),
			mage::Vertex(mage::Position(0.5f, 0.5f, 0.5f), mage::Normal(0, 1, 0), mage::TexCoords(1, 0)),
			mage::Vertex(mage::Position(0.5f, 0.5f, 0.5f), mage::Normal(0, 1, 0), mage::TexCoords(1, 0)),
			mage::Vertex(mage::Position(-0.5f, 0.5f, 0.5f), mage::Normal(0, 1, 0), mage::TexCoords(0, 0)),
			mage::Vertex(mage::Position(-0.5f, 0.5f, -0.5f), mage::Normal(0, 1, 0), mage::TexCoords(0, 1)),
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

	mage::clearConsole(mage::console::WHITE);
	mage::Input input;
	const mage::Display display("Mage3D Testing", 1920, 1080, &input);
	mage::Camera camera(&display, glm::vec3(0, 0, 5));
	//auto* testMesh = new mage::Mesh(vertices, 5, indices, 18);

	mage::Mesh lampMesh(testVerts, testInts);


	mage::Renderer renderer;

	mage::Shader shader("./assets/shaders/basic_lighting");
	mage::Shader shader2("./assets/shaders/basic_lighting.vert", "./assets/shaders/basic_model.frag");
	mage::Shader shader3("./assets/shaders/lighting");
	mage::Shader shaderLamp("./assets/shaders/basic");
	mage::Texture texture("./assets/textures/default.png");
	list<mage::Texture> planeTextures = { texture };
	//mage::Mesh planeMesh(planeVerts, planeInts, planeTextures);
	mage::Mesh cubeMesh(verts, ints, planeTextures);
	mage::Model backpack("./assets/models/untitled.obj");
	mage::Model backpackActual("./assets/models/backpack.obj");
	mage::Model bricks("./assets/models/bricks.obj");

	vec3f lightPos(0, -0.9f, 0);



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

		//shader.enable();
		//
		//glm::mat4 rot(1);
		//rot = glm::rotate((float) rotTimer.elapsed() * 0.6f, glm::vec3(1, 1, 1));
		//shader.setUniformMatf("model", rot);
		//shader.setUniformMatf("projection", camera.getProjectionMatrix());
		//shader.setUniformMatf("view", camera.getViewMatrix());
		//
		//shader.setUniform3f("lightColor", vec3f(1.0f, 0.5f, 0.7f));
		//shader.setUniform3f("lightPos", lightPos);
		//shader.setUniform3f("viewPos", camera.getPosition());
		//
		//renderer.render(shader, cubeMesh);
		//
		//shader.disable();

		//shader.enable();
		//glm::mat4 plane(1);
		//shader.setUniformMatf("model", plane);
		//shader.setUniformMatf("projection", camera.getProjectionMatrix());
		//shader.setUniformMatf("view", camera.getViewMatrix());
		//shader.setUniform3f("lightColor", vec3f(1.0f, 0.5f, 0.7f));
		//shader.setUniform3f("lightPos", lightPos);
		//shader.setUniform3f("viewPos", camera.getPosition());
		//
		//renderer.render(shader, planeMesh);
		//shader.disable();


		shader3.enable();

		glm::mat4 bp = glm::scale(glm::mat4(1), glm::vec3(1));
		bp = glm::translate(bp, vec3f(0, 0, -4));
		glm::mat4 v = glm::transpose(glm::inverse(bp * camera.getViewMatrix()));
		//bp = glm::rotate(bp, (float) rotTimer.elapsed() * 0.6f, glm::vec3(1, 1, 1));
		shader3.setUniformMatf("model", bp);
		shader3.setUniformMatf("projection", camera.getProjectionMatrix());
		shader3.setUniformMatf("view", camera.getViewMatrix());
		shader3.setUniformMatf("normalMatrix", v);
		shader3.setUniform3f("material.ambient", vec3f(1, 0.5f, 0.31f));
		shader3.setUniform3f("material.diffuse", vec3f(1, 0.5f, 0.31f));
		shader3.setUniform3f("material.specular", vec3f(0.5f, 0.5f, 0.5f));
		shader3.setUniform1f("material.shininess", 32.0f);
		shader3.setUniform3f("light.ambient", vec3f(0.2f));
		shader3.setUniform3f("light.diffuse", vec3f(0.5f));
		shader3.setUniform3f("light.specular", vec3f(1));
		shader3.setUniform3f("lightPos", vec3f(0, 0, -1));


		renderer.render(shader3, backpackActual);

		shader3.disable();

		shader3.enable();

		glm::mat4 br = glm::translate(glm::mat4(1), vec3f(-4, 0, 0));
		br = glm::scale(br, glm::vec3(0.5f));
		//br = glm::rotate(br, (float) rotTimer.elapsed() * 0.6f, glm::vec3(1, 1, 1));
		glm::mat4 v2 = glm::transpose(glm::inverse(br * camera.getViewMatrix()));
		//bp = glm::rotate(bp, (float) rotTimer.elapsed() * 0.6f, glm::vec3(1, 1, 1));
		shader3.setUniformMatf("model", br);
		shader3.setUniformMatf("projection", camera.getProjectionMatrix());
		shader3.setUniformMatf("view", camera.getViewMatrix());
		shader3.setUniformMatf("normalMatrix", v2);
		//shader3.setUniform3f("material.ambient", vec3f(1, 0.5f, 0.31f));
		//shader3.setUniform3f("material.diffuse", vec3f(1, 0.5f, 0.31f));
		//shader3.setUniform3f("material.specular", vec3f(0.5f, 0.5f, 0.5f));
		shader3.setUniform1f("material.shininess", 32.0f);
		shader3.setUniform3f("light.ambient", vec3f(0.1f));
		shader3.setUniform3f("light.diffuse", vec3f(0.5f));
		shader3.setUniform3f("light.specular", vec3f(1));
		shader3.setUniform3f("lightPos", vec3f(0, 0, -1));


		renderer.render(shader3, bricks);

		shader3.disable();

		shaderLamp.enable();
		//glm::mat4 model = glm::translate(glm::mat4(1), lightPos);
		shaderLamp.setUniformMatf("model", glm::mat4(1));
		shaderLamp.setUniformMatf("projection", camera.getProjectionMatrix());
		shaderLamp.setUniformMatf("view", camera.getViewMatrix());

		renderer.render(shaderLamp, lampMesh);

		shaderLamp.disable();

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

	return 0;
}
