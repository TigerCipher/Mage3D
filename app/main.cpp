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

#define BMD_PROFILE 1

#include <bmd/profiler.h>
#include <fmt/format.h>

class TestGame : public mage::Game
{
public:
	TestGame() : Game() {}

	void init() override
	{
		shaderBasicLighting = createRef<mage::Shader>("./assets/shaders/lighting");
		shaderLamp = createRef<mage::Shader>("./assets/shaders/basic");
		cube = createRef<mage::Model>("./assets/models/bricks.obj");
		backpack = createRef<mage::Model>("./assets/models/backpack.obj");
		brickDif = createRef<mage::Texture>(
				"./assets/textures/bricks_diffuse.png", TEXTURE_DIFFUSE);
		brickSpec = createRef<mage::Texture>(
				"./assets/textures/bricks_specular.png", TEXTURE_SPECULAR);
		backDif = createRef<mage::Texture>(
				"./assets/textures/backpack_diffuse.jpg", TEXTURE_DIFFUSE);
		backSpec = createRef<mage::Texture>(
				"./assets/textures/backpack_specular.jpg", TEXTURE_SPECULAR);

		brickMat = mage::Material { brickDif.get(), brickSpec.get(), nullptr, 32.0f };
		backMat = mage::Material { backDif.get(), backSpec.get(), nullptr, 32.0f };

		lightPos = vec3f(0, -0.9f, 0);

		for (int i = 0; i < 20; i++)
		{
			mat4f m = glm::translate(mat4f(1), vec3f((float) i * 3.5f, 0, -4));
			backpackTransformations.push_back(m);
		}
		brickTransformation = glm::translate(mat4f(1), vec3f(-4, 0, 0));
		brickTransformation = glm::scale(brickTransformation, vec3f(0.5f));
		lampTransformation = glm::scale(mat4f(1), vec3f(0.1f));
	}

	void processInput(mage::Input* input, float delta) override
	{
		m_camera.update(*input, delta);
		if(input->keyPressed(KEY_ESCAPE)){
			mage::Display::toggleCursor();
		}
	}

	void update(float delta) override {
		brickTransformation = glm::rotate(brickTransformation, delta * 0.5f, vec3f(0, 1, 1));
	}

	void render() override
	{
		shaderBasicLighting->enable();

		for (int i = 0; i < 20; i++)
		{
			glm::mat4 v = glm::transpose(
					glm::inverse(backpackTransformations[ i ] * m_camera.getViewMatrix()));
			//bp = glm::rotate(bp, (float) rotTimer.elapsed() * 0.6f, glm::vec3(1, 1, 1));
			shaderBasicLighting->setUniformMatf("model", backpackTransformations[ i ]);
			shaderBasicLighting->setUniformMatf("projection", m_camera.getProjectionMatrix());
			shaderBasicLighting->setUniformMatf("view", m_camera.getViewMatrix());
			shaderBasicLighting->setUniformMatf("normalMatrix", v);
			shaderBasicLighting->setUniform3f("light.ambient", vec3f(0.1f));
			shaderBasicLighting->setUniform3f("light.diffuse", vec3f(0.5f));
			shaderBasicLighting->setUniform3f("light.specular", vec3f(1));
			shaderBasicLighting->setUniform3f("lightPos", vec3f(0, 0, -1));


			renderer.render(*shaderBasicLighting, *backpack, backMat);
		}
		shaderBasicLighting->disable();

		shaderBasicLighting->enable();

		glm::mat4 v2 = glm::transpose(glm::inverse(brickTransformation * m_camera.getViewMatrix()));

		//TODO This can probably be put under some form a transformation class/struct
		shaderBasicLighting->setUniformMatf("model", brickTransformation);
		shaderBasicLighting->setUniformMatf("projection", m_camera.getProjectionMatrix());
		shaderBasicLighting->setUniformMatf("view", m_camera.getViewMatrix());
		shaderBasicLighting->setUniformMatf("normalMatrix", v2);
		//shader3.setUniform3f("material.ambient", vec3f(1, 0.5f, 0.31f));
		//shader3.setUniform3f("material.diffuse", vec3f(1, 0.5f, 0.31f));
		//shader3.setUniform3f("material.specular", vec3f(0.5f, 0.5f, 0.5f));
		//shader3.setUniform1f("material.shininess", 32.0f);
		// TODO This should be under a light struct/class
		shaderBasicLighting->setUniform3f("light.ambient", vec3f(0.1f));
		shaderBasicLighting->setUniform3f("light.diffuse", vec3f(0.5f));
		shaderBasicLighting->setUniform3f("light.specular", vec3f(1));
		shaderBasicLighting->setUniform3f("lightPos", vec3f(0, 0, -1));


		renderer.render(*shaderBasicLighting, *cube, brickMat);

		shaderBasicLighting->disable();

		shaderLamp->enable();
		shaderLamp->setUniformMatf("model", lampTransformation);
		shaderLamp->setUniformMatf("projection", m_camera.getProjectionMatrix());
		shaderLamp->setUniformMatf("view", m_camera.getViewMatrix());

		renderer.render(*shaderLamp, *cube);

		shaderLamp->disable();
	}

	void destroy() override { }

private:
	list<mat4f> backpackTransformations;
	mat4f brickTransformation{};
	mat4f lampTransformation{};
	mage::Camera m_camera;

	mage::Renderer renderer;

	SharedPtr<mage::Shader> shaderBasicLighting;
	SharedPtr<mage::Shader> shaderLamp;
	SharedPtr<mage::Model> cube;
	SharedPtr<mage::Model> backpack;

	SharedPtr<mage::Texture> brickDif;
	SharedPtr<mage::Texture> brickSpec;
	SharedPtr<mage::Texture> backDif;
	SharedPtr<mage::Texture> backSpec;
	mage::Material brickMat{};
	mage::Material backMat{};
	vec3f lightPos{};


};

int main(int argc, char** argv)
{
	TestGame game;
	mage::Engine engine(&game, "Test Game", 1920, 1080);
	engine.start();
	return 0;
}
