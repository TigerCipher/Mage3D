/*
 * Mage3D
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
 * File Name: engine.cpp
 * Date File Created: 8/12/2020 at 11:22 PM
 * Author: Matt
 */

#include "mage/core/engine.h"
#include "mage/events/dummyevent.h"
#include "mage/core/assetmanager.h"

mage::Engine* mage::Engine::s_instance;

mage::Engine::Engine(Game* game, const char* title, int width, int height, float ticksPerSecond,
                     bool limitFrames) :
        m_running(false),
        m_game(game),
        m_msPerUpdate(1.0f / ticksPerSecond),
        m_limitFrames(limitFrames)
{
    PROFILER_SCOPE(1);
    Log::init();
    LOG_INFO("Initializing mage3d engine");
    m_dispatcher = createScope<EventDispatcher>("Mage3D Core Events", 1);
    LOG_TRACE("Event dispatcher created");
    SharedPtr<DummyEvent> preInitEvent = createRef<DummyEvent>(PRE_INIT);
    m_dispatcher->dispatch(preInitEvent);
    // TODO: See if possible to make the GL /glfw contexts but not show window until after loading resources
    // Or change assets system to load data (i.e loading models into vertices, indices, etc) and waiting to bind to gl buffers until after
    // gl contexts are created
    Display::create(title, width, height, &m_input);
    LOG_TRACE("Display created [title = {}, size: {} x {}]", title, width, height);
    AssetManager::loadAssets("./assets");
    m_renderEngine = new RenderEngine();
    LOG_TRACE("Render engine created");
    m_screenShader = AssetManager::getShader("./assets/shaders/screen");
    list<float> quadVertices = {
            // positions   // texCoords
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
    };
    m_quad = new Mesh(quadVertices);
    //m_renderEngine = createRef<const RenderEngine>();
}

mage::Engine::~Engine()
{
    PROFILER_SCOPE(1);
    LOG_INFO("Cleaning up engine resources");
    delete m_quad;
    delete m_renderEngine;
    AssetManager::destroy();
    Display::destroy();
}

void mage::Engine::start()
{
    if (m_running) return;
    SharedPtr<DummyEvent> initEvent = createRef<DummyEvent>(INIT);
    m_dispatcher->dispatch(initEvent);
    m_game->init();
    m_running = true;
    run();
}

void mage::Engine::stop()
{
    if (!m_running) return;
    m_running = false;
}

void mage::Engine::run()
{
    SharedPtr<DummyEvent> postInitEvent = createRef<DummyEvent>(POST_INIT);
    m_dispatcher->dispatch(postInitEvent);
    timer_t timer;
    initTimer(&timer);
    float accumulated = 0;
    int loops = 0;
    int ticks = 0;
    float frameTime = 0;
    while (m_running && !Display::isClosed())
    {
        tickTimer(&timer);

        calculateFrameStatistics(frameTime, ticks);
        accumulated += timer.delta;
        frameTime += timer.delta;

        // TODO: Best to process input and movement with variable time step or should use msPerUpdate / fixed timestep in side the below loop?
        m_game->processInput(&m_input, timer.delta);
        loops = 0;
        while (accumulated >= m_msPerUpdate &&
               loops < 10) //TODO make 10 (max frames allowed to skip render) modifiable
        {
            ticks++;
            loops++;
            m_game->update(m_msPerUpdate);
            accumulated -= m_msPerUpdate;
        }

        m_renderEngine->getTarget()->bindAsRenderTarget();
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        Display::clear();

        // TODO: Might want to interpolate positions to render at between 2 ticks - accumulated / m_msPerUpdate should suffice
        glEnable(GL_DEPTH_TEST);
        m_game->render(m_renderEngine);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, Display::getWidth(), Display::getHeight());

        m_screenShader.enable();
        m_quad->enable_basic();
        glBindTexture(GL_TEXTURE_2D, m_renderEngine->getTarget()->getData()->getId());
        //m_screenShader.setUniform1i("screenTexture", 0);
        m_quad->draw_basic();
        m_quad->disable_basic();
        glBindTexture(GL_TEXTURE_2D, 0);
        m_screenShader.disable();

        m_input.update();
        Display::update();
    }

    delete s_instance;
}

void mage::Engine::calculateFrameStatistics(float& elapsed, int& ticks)
{
    static int frames = 0;
    frames++;

    if (elapsed >= 1.0f)
    {
        float mspf = 1000.0f / (float) frames;
        DBGPRINT("FPS: %i -- mspf: %f --- ticks: %i", frames, mspf, ticks);
        frames = 0;
        elapsed = 0;
        ticks = 0;
    }
}


void
mage::Engine::createGame(mage::Game* game, const char* title, int width, int height, float ticksPerSecond,
                         bool limitFrames)
{
    s_instance = new Engine(game, title, width, height, ticksPerSecond, limitFrames);
    s_instance->start();
}

