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
 * File Name: dummyevent.h
 * Date File Created: 8/13/2020 at 11:18 PM
 * Author: Matt
 */

#ifndef MAGE3D_DUMMYEVENT_H
#define MAGE3D_DUMMYEVENT_H

#include "mage3d_exported.h"
#include "mage/common.h"
#include "event.h"


namespace mage
{
	class DummyEvent : public Event
	{

	public:
		mage3d_EXPORT explicit DummyEvent(EventType type) : Event(type) {}
		mage3d_EXPORT ~DummyEvent() override = default;

		void invoke() override
		{
			// Do nothing, dummy event (for now at least)
		}
	};
}

#endif //MAGE3D_DUMMYEVENT_H
