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
// File Name: timer.h
// Date File Created: 07/09/2020 at 2:09 AM
// Author: Matt / TigerCipher

#pragma once
#include <mage3d_exported.h>

class Timer
{
public:
	mage3d_EXPORT Timer();

	mage3d_EXPORT void reset();

	mage3d_EXPORT double elapsed();

private:
	double m_time;
};
