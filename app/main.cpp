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


class TestScene : public mage::Scene
{
public:
    void init() override
    {
        PROFILER_SCOPE(1);
        auto* cam = (new mage::GameObject())->addComponent(new mage::Camera());
        addToScene(cam);
        brick = new mage::GameObject();
        auto* lamp = new mage::GameObject();
        SharedPtr<mage::Model> backpack = createRef<mage::Model>("./assets/models/backpack.obj");
        SharedPtr<mage::Model> cube = createRef<mage::Model>("./assets/models/bricks.obj");
        auto* brickDif = new mage::Texture("./assets/textures/bricks_diffuse.png", TEXTURE_DIFFUSE);
        auto* brickSpec = new mage::Texture("./assets/textures/bricks_specular.png", TEXTURE_SPECULAR);
        auto* backDif = new mage::Texture("./assets/textures/backpack_diffuse.jpg", TEXTURE_DIFFUSE);
        auto* backSpec = new mage::Texture("./assets/textures/backpack_specular.jpg", TEXTURE_SPECULAR);
        SharedPtr<mage::Material> brickMat = createRef<mage::Material>(brickDif, brickSpec, nullptr, 32.0f);
        SharedPtr<mage::Material> backMat = createRef<mage::Material>(backDif, backSpec, nullptr, 32.0f);

        vec3f lightPos(0, 0.5f, -1.5f);

        for (int i = 0; i < 20; i++)
        {
            auto* backp = (new mage::GameObject())->addComponent(
                    new mage::ModelRenderer(backpack, backMat, lightPos));
            backp->getTransform().setPos(vec3f((float) i * 3.5f, 0, -4));
            addToScene(backp);
        }

        brick->addComponent(new mage::ModelRenderer(cube, brickMat, lightPos));
        brick->getTransform().setPos(vec3f(-4, 0, 0));
        brick->getTransform().setScale(0.5f);

        lamp->addComponent(new mage::BasicModelRenderer(cube));
        lamp->getTransform().setPos(lightPos);
        lamp->getTransform().setScale(0.1f);

        addToScene(brick);
        addToScene(lamp);
    }

    void update(float delta) override
    {
        brick->getTransform().rotate(delta * 0.5f, vec3f(0, 1, 1));
        mage::Scene::update(delta);
    }

private:
    mage::GameObject* brick{};
};

class TestGame : public mage::Game
{
public:
    TestGame() :
            Game() { }

    void init() override
    {
        setCurrentScene(&m_scene);
    }

    void processInput(mage::Input* _input, float delta) override
    {
        if (_input->keyPressed(KEY_ESCAPE))
        {
            mage::Display::toggleCursor();
        }
        mage::Game::processInput(_input, delta);
    }

private:
    TestScene m_scene;
};

int main(int argc, char** argv)
{
    TestGame game;
    mage::Engine::createGame(&game, "Test Game");
    return 0;
}
