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
 * File Name: eventdispatcher.cpp
 * Date File Created: 8/13/2020 at 10:57 PM
 * Author: Matt
 */

#include "mage/events/eventdispatcher.h"


mage::EventDispatcher::EventDispatcher(std::string name, int threadCount) :
		m_name(std::move(name)),
		m_threads(threadCount)
{
	for (auto& thread : m_threads)
	{
		thread = std::thread(&EventDispatcher::dispatchThreadHandler, this);
	}
}

mage::EventDispatcher::~EventDispatcher()
{
	std::unique_lock<std::mutex> lock(m_lock);
	m_quit = true;
	lock.unlock();
	m_cv.notify_all();

	for (auto& thread : m_threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
}

void mage::EventDispatcher::dispatch(const Ref<mage::Event>& event)
{
	std::unique_lock<std::mutex> lock(m_lock);
	m_queue.push(event);
	lock.unlock();
	m_cv.notify_one();
}

void mage::EventDispatcher::addListener(const Ref<mage::EventListener>& listener)
{
	m_listeners.push_back(listener);
}

void mage::EventDispatcher::removeListener(const Ref<mage::EventListener>& listener)
{
	std::vector<Ref<EventListener>>::const_iterator it;
	for (auto iterator = m_listeners.begin(); iterator != m_listeners.end(); ++iterator)
	{
		if ((*iterator) == listener)
		{
			iterator = m_listeners.erase(iterator);
		}
	}
}

void mage::EventDispatcher::dispatchThreadHandler()
{
	std::unique_lock<std::mutex> lock(m_lock);
	do
	{
		m_cv.wait(lock, [ this ]
		{
			return (!m_queue.empty() || m_quit);
		});

		if (!m_queue.empty())
		{
			auto op = m_queue.front();
			m_queue.pop();
			lock.unlock();
			op->invoke();
			for (const auto& listener : m_listeners)
			{
				listener->onEvent(op);
			}
			lock.lock();
		}

	} while (!m_quit);
}
