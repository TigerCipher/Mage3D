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
 * File Name: game.h
 * Date File Created: 8/12/2020 at 11:24 PM
 * Author: Matt
 */

#ifndef MAGE3D_GAME_H
#define MAGE3D_GAME_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "input.h"
#include "mage/graphics/renderengine.h"


namespace mage
{
	class Game
	{
	public:
		mage3d_EXPORT Game() = default;
		mage3d_EXPORT virtual void init() {}
		mage3d_EXPORT virtual void processInput(Input* input, float delta) {}
		mage3d_EXPORT virtual void update(float delta) {}
		mage3d_EXPORT virtual void render(const RenderEngine* renderEngine) {}
		mage3d_EXPORT virtual void destroy() {}

		mage3d_EXPORT inline void setPaused(bool paused)
		{
			m_paused = paused;
		}

		mage3d_EXPORT inline bool isPaused()
		{
			return m_paused;
		}
	protected:
	private:
		Game(Game& game) = default;
        void operator=(Game& game) {}
        bool m_paused = false;
	};

}


#endif //MAGE3D_GAME_H
