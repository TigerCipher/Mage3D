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
 * File Name: eventdispatcher.h
 * Date File Created: 8/13/2020 at 10:57 PM
 * Author: Matt
 */

#ifndef MAGE3D_EVENTDISPATCHER_H
#define MAGE3D_EVENTDISPATCHER_H


#include "mage3d_exported.h"
#include "mage/common.h"
#include "event.h"
#include "eventlistener.h"

#include <string>
#include <thread>
#include <queue>
#include <mutex>

namespace mage
{
	class EventDispatcher
	{
	public:
		mage3d_EXPORT explicit EventDispatcher(std::string name, int threadCount = 1);

		mage3d_EXPORT virtual ~EventDispatcher();

		mage3d_EXPORT void dispatch(const Ref<Event>& event);
		mage3d_EXPORT void addListener(const Ref<EventListener>& listener);
		mage3d_EXPORT void removeListener(const Ref<EventListener>& listener);

		EventDispatcher(const EventDispatcher& rhs) = delete;
		EventDispatcher& operator=(const EventDispatcher& rhs) = delete;
		EventDispatcher(EventDispatcher&& rhs) = delete;
		EventDispatcher& operator=(EventDispatcher&& rhs) = delete;

	protected:
	private:
		void dispatchThreadHandler();

		std::string m_name;
		std::mutex m_lock;
		list<std::thread> m_threads;
		list<Ref<EventListener>> m_listeners;
		std::queue<Ref<Event>> m_queue;
		std::condition_variable m_cv;
		bool m_quit = false;
	};

}


#endif //MAGE3D_EVENTDISPATCHER_H
