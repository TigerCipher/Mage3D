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
// File Name: main.cpp
// Date File Created: 07/11/2020 at 4:19 PM
// Author: Matt / TigerCipher

#include <mage/lib.h>
//#define FMT_HEADER_ONLY
#include <iostream>
#include <fmt/format.h>
#include <fmt/color.h>

int main(int argc, char** argv)
{

	Test test;
	fmt::print("This is just a simple test named {} and stuff\n", "Johny Appleseed");
	
	fmt::print("Testing glew... received error code: {}\n", test.glewTest(13));
	
	fmt::print(stderr, "Test error {}", 43);
	print(fg(fmt::color::red) | bg(fmt::color::blue), "The color is red and green\n");

	std::cout << "Press any key and then enter to continue... ";
	char input;
	std::cin >> input;

	return 0;
}
