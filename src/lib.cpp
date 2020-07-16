// TestProject
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
// File Name: lib.cpp
// Date File Created: 07/11/2020 at 4:26 PM
// Author: Matt / TigerCipher

#include <mage/lib.h>
#include <mage3d_exported.h>
#include <mage/test/testheader.h>

mage3d_EXPORT Test::Test()
{
	print("This is a sub folder header test");
}

mage3d_EXPORT GLenum Test::glewTest(int test)
{
	const int error = glewInit();
	return error;
}
