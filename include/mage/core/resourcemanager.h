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
 * File Name: resourcemanager.h
 * Date File Created: 8/4/2020 at 9:09 PM
 * Author: Matt
 */

#ifndef MAGE3D_RESOURCEMANAGER_H
#define MAGE3D_RESOURCEMANAGER_H


#include "mage3d_exported.h"
#include "mage/common.h"

#include <map>
#include <string>


namespace mage
{

	class ResourceManager
	{
	public:
		ResourceManager() :
				m_count(0) { }

		~ResourceManager() = default;


		void addReference()
		{
			m_count++;
		}

		bool removeReference()
		{
			m_count--;
			return m_count == 0;
		}


		uint getResourceCount() const { return m_count; }

	protected:
	private:
		uint m_count;
	};


}


#endif //MAGE3D_RESOURCEMANAGER_H
