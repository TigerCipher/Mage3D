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
 * File Name: eventlistener.h
 * Date File Created: 8/13/2020 at 10:54 PM
 * Author: Matt
 */

#ifndef MAGE3D_EVENTLISTENER_H
#define MAGE3D_EVENTLISTENER_H

#include "mage3d_exported.h"
#include "mage/common.h"
#include "event.h"

namespace mage
{
	class EventListener
	{
	public:
		mage3d_EXPORT EventListener() = default;
		mage3d_EXPORT virtual ~EventListener() = default;

		mage3d_EXPORT virtual void onEvent(const SharedPtr<Event>& event) = 0;
	};
}

#endif //MAGE3D_EVENTLISTENER_H
