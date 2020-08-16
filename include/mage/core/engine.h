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
 * File Name: engine.h
 * Date File Created: 8/12/2020 at 11:22 PM
 * Author: Matt
 */

#ifndef MAGE3D_ENGINE_H
#define MAGE3D_ENGINE_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "display.h"
#include "game.h"
#include "input.h"
#include "mage/events/eventdispatcher.h"
#include "mage/graphics/renderengine.h"

namespace mage
{
	class Engine
	{
	public:
		mage3d_EXPORT Engine(Game* game, const char* title, int width, int height, float ticksPerSecond = 120.0f, bool limitFrames = false);

		mage3d_EXPORT virtual ~Engine();

		mage3d_EXPORT void start();
		mage3d_EXPORT void stop();

	protected:
	private:

		void run();
		static void calculateFrameStatistics(float& elapsed, int& ticks);
		bool m_running;
		Game* m_game;
		Input m_input;
		float m_msPerUpdate;
		bool m_limitFrames;
		UniquePtr<EventDispatcher> m_dispatcher;
		const RenderEngine* m_renderEngine;
	};

}


#endif //MAGE3D_ENGINE_H
