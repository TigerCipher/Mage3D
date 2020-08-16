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
    TestGame() :
            Game() { }

    void init() override
    {
        PROFILER_SCOPE(1);
        m_root = createRef<mage::GameObject>();
        m_basic = createRef<mage::GameObject>();

        brick = new mage::GameObject();
        auto* lamp = new mage::GameObject();
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

        brickMat = createRef<mage::Material>(brickDif.get(), brickSpec.get(), nullptr, 32.0f);
        backMat = createRef<mage::Material>(backDif.get(), backSpec.get(), nullptr, 32.0f);

        lightPos = vec3f(0, 0.5f, -1.5f);
        m_camera = createRef<mage::Camera>();


        for (int i = 0; i < 20; i++)
        {
            auto* backp = (new mage::GameObject())->addComponent(
                    new mage::ModelRenderer(backpack, backMat, m_camera, lightPos, mage::LIGHTING));
            backp->getTransform().setPos(vec3f((float) i * 3.5f, 0, -4));
            m_root->addChild(backp);
        }

        brick->addComponent(new mage::ModelRenderer(cube, brickMat, m_camera, lightPos, mage::LIGHTING));
        brick->getTransform().setPos(vec3f(-4, 0, 0));
        brick->getTransform().setScale(0.5f);

        lamp->addComponent(new mage::ModelRenderer(cube, nullptr, m_camera, lightPos, mage::LAMP));
        lamp->getTransform().setPos(lightPos);
        lamp->getTransform().setScale(0.1f);

        m_root->addChild(brick);
        m_basic->addChild(lamp);

    }

    void processInput(mage::Input* input, float delta) override
    {
        m_camera->update(*input, delta);
        if (input->keyPressed(KEY_ESCAPE))
        {
            mage::Display::toggleCursor();
        }
        m_root->inputAll(input, delta);
        m_basic->inputAll(input, delta);

    }

    void update(float delta) override
    {
        brick->getTransform().rotate(delta * 0.5f, vec3f(0, 1, 1));
        m_root->updateAll(delta);
        m_basic->updateAll(delta);
    }

    void render() override
    {

        m_root->renderAll(&renderer, shaderBasicLighting.get());
        m_basic->renderAll(&renderer, shaderLamp.get());
    }

    void destroy() override { }

private:
    mage::GameObject* brick;
    SharedPtr<mage::GameObject> m_root;
    SharedPtr<mage::GameObject> m_basic;
    SharedPtr<mage::Camera> m_camera;

    mage::Renderer renderer;

    SharedPtr<mage::Shader> shaderBasicLighting;
    SharedPtr<mage::Shader> shaderLamp;
    SharedPtr<mage::Model> cube;
    SharedPtr<mage::Model> backpack;

    SharedPtr<mage::Texture> brickDif;
    SharedPtr<mage::Texture> brickSpec;
    SharedPtr<mage::Texture> backDif;
    SharedPtr<mage::Texture> backSpec;

    SharedPtr<mage::Material> brickMat { };
    SharedPtr<mage::Material> backMat { };
    vec3f lightPos { };


};

int main(int argc, char** argv)
{
    TestGame game;
    mage::Engine engine(&game, "Test Game", 1920, 1080);
    engine.start();
    return 0;
}
