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
// File Name: main.cpp
// Date File Created: 07/11/2020 at 4:19 PM
// Author: Matt / TigerCipher


#include <iostream>
#include <string>
#include <io.h>
#include <mage/mage.h>

#ifdef WIN32 //|| _WIN32 || _WIN64 || _WINDOWS
#include <Windows.h>
#endif // WIN32

int main(int argc, char** argv)
{

    mage::std::clearConsole(mage::std::RED_ON_GREEN);
    mage::std::print(mage::std::RED, "This is just a test");

	const Display display("Game Engine", 1920, 1080);


	Timer timer;
	int frames = 0;

	while (!display.isClosed())
	{
		display.clear(255, 0, 0);

		display.update();

		if (timer.elapsed() >= 1.0)
		{
			// printf("FPS: %i\n", frames);
			mage::std::print(mage::std::RED, "FPS: {}\n", frames);
			// fmt::print(fg(fmt::terminal_color::bright_cyan), "FPS: {}\n", frames);
			frames = 0;
			timer.reset();
		}
		else frames++;
	}

	return 0;
}
