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
 * File Name: event.h
 * Date File Created: 8/13/2020 at 10:35 PM
 * Author: Matt
 */

#ifndef MAGE3D_EVENT_H
#define MAGE3D_EVENT_H

#include "mage3d_exported.h"
#include "mage/common.h"


namespace mage
{
	enum EventType
	{
		PRE_INIT, INIT, POST_INIT
	};

	class Event
	{
	public:
		mage3d_EXPORT explicit Event(EventType type) : m_type(type) {}
		mage3d_EXPORT virtual ~Event() = default;
		mage3d_EXPORT virtual void invoke() {}

		mage3d_EXPORT inline EventType getType() { return m_type; }
	protected:
		EventType m_type;
	};
}

#endif //MAGE3D_EVENT_H
