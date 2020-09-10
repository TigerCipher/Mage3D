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

        list<std::string> skyboxFaces = {
                "./assets/skybox/right.jpg",
                "./assets/skybox/left.jpg",
                "./assets/skybox/top.jpg",
                "./assets/skybox/bottom.jpg",
                "./assets/skybox/front.jpg",
                "./assets/skybox/back.jpg"
        };

        initSkybox(skyboxFaces);

        auto* cam = (new mage::GameObject())->addComponent(new mage::Camera());
        addToScene(cam);
        brick = new mage::GameObject();
        auto* lamp = new mage::GameObject();
        //mage::Model backpack("./assets/models/backpack.obj",
        //                     mage::Material(
        //                             "./assets/textures/backpack_diffuse.jpg",
        //                             "./assets/textures/backpack_specular.jpg"));
        mage::Model backpack("./assets/models/backpack.obj", mage::AssetManager::getMaterial("backpack"));
        mage::Model cube("./assets/models/bricks.obj", mage::AssetManager::getMaterial("bricks"));

        mage::Model plane("./assets/models/plane.obj", mage::AssetManager::getMaterial("default"));
        mage::Model window("./assets/models/plane.obj", mage::Material("./assets/textures/red_window.png"));

        vec3f lightPos(0, 0.5f, -1.5f);
        auto* planeObj = (new mage::GameObject())->addComponent(new mage::ModelRenderer(plane, lightPos));
        planeObj->getTransform().setPos(vec3f(0, -5, 0));
        addToScene(planeObj);
        //auto* windowPane = (new mage::GameObject())->addComponent(new mage::ModelRenderer(window, lightPos));
        //windowPane->getTransform().setScale(0.25f);
        //windowPane->getTransform().rotate(glm::radians(90.0f), vec3f(1, 0, 0));
        //addToScene(windowPane);


        for (int i = 0; i < 20; i++)
        {
            auto* backp = (new mage::GameObject())->addComponent(
                    new mage::ModelRenderer(backpack, lightPos));
            backp->getTransform().setPos(vec3f((float) i * 3.5f, 0, -4));
            addToScene(backp);
        }

        brick->addComponent(new mage::ModelRenderer(cube, lightPos));
        brick->getTransform().setPos(vec3f(-4, 0, 0));
        brick->getTransform().setScale(0.5f);

        lamp->addComponent(new mage::BasicModelRenderer(cube));
        lamp->getTransform().setPos(lightPos);
        lamp->getTransform().setScale(0.1f);

        for (int i = 1; i <= 3; i++)
        {
            auto* lampObj = (new mage::GameObject())->addComponent(new mage::BasicModelRenderer(cube));
            lampObj->getTransform().setPos(lightPos + vec3f(10 * i, 0, 0));
            lampObj->getTransform().setScale(0.1f);
            addToScene(lampObj);
        }
        //m_lightPos + vec3f(10, 5, 0)
        //m_lightPos + vec3f(-20, 20, 10)
        //m_lightPos + vec3f(30, 0, -10)

        addToScene(brick);
        addToScene(lamp);

        // TODO CUrrently must be added last. Need to come up with an efficient sorting algorithm
        auto* winPane = (new mage::GameObject())
                ->addComponent(new mage::QuadRenderer("./assets/textures/red_window.png"));
        winPane->getTransform().rotate(-glm::radians(90.0f), vec3f(1, 0, 0));
        winPane->getTransform().setPos(vec3f(0, 0, 0));
        addToScene(winPane);
    }

    void update(float delta) override
    {
        brick->getTransform().rotate(delta * 0.5f, vec3f(0, 1, 1));
        mage::Scene::update(delta);
    }


private:
    mage::GameObject* brick { };
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
        if (_input->keyPressed(KEY_F))
        {
            if (m_toggle >= 6) m_toggle = -1;
            else if (m_toggle < 0) m_toggle = 1;
            else m_toggle++;
            m_toggled = true;
        }
        mage::Game::processInput(_input, delta);
    }

    void render(const mage::RenderEngine* renderEngine) override
    {
        if (m_toggle == 1 && m_toggled)
        {
            renderEngine->setFilter(mage::AssetManager::getShader("inversion"));
            m_toggled = false;
        } else if (m_toggle == -1 && m_toggled)
        {
            renderEngine->setFilter(mage::AssetManager::getShader("screen"));
            m_toggled = false;
        } else if (m_toggle == 2 && m_toggled)
        {
            renderEngine->setFilter(mage::AssetManager::getShader("grayscale"));
            m_toggled = false;
        } else if (m_toggle == 3 && m_toggled)
        {
            renderEngine->setFilter(mage::AssetManager::getShader("sharpen"));
            m_toggled = false;
        } else if (m_toggle == 4 && m_toggled)
        {
            renderEngine->setFilter(mage::AssetManager::getShader("blur"));
            m_toggled = false;
        } else if (m_toggle == 5 && m_toggled)
        {
            renderEngine->setFilter(mage::AssetManager::getShader("edges"));
            m_toggled = false;
        } else if (m_toggle == 6 && m_toggled)
        {
            renderEngine->setFilter(mage::AssetManager::getShader("greenscale"));
            m_toggled = false;
        }
        Game::render(renderEngine);
    }

private:
    TestScene m_scene;
    int m_toggle = 0;
    bool m_toggled = false;
};

int main(int argc, char** argv)
{
    TestGame game;
    mage::Engine::createGame(&game, "Test Game");
    return 0;
}
