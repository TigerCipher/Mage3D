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


#include <string>
#include <mage/mage.h>

int main(int argc, char** argv)
{
	mage::Vertex vert1;
	mage::Vertex vert2;
	mage::Vertex vert3;
	vert1.setPos(-1, -1, 0);
	vert1.setColor(1, 0, 0);
	vert2.setPos(1, -1, 0);
	vert2.setColor(0, 1, 0);
	vert3.setPos(0, 1, 0);
	vert3.setColor(0, 0, 1);
	mage::Vertex data[] = {
			vert1, vert2, vert3
	};
    mage::clearConsole(mage::console::WHITE);
	const mage::Display display("Mage3D Testing", 1920, 1080);
	mage::Mesh* testMesh = new mage::Mesh(data);
	mage::Renderer renderer;

	mage::Shader* shader = new mage::Shader("./assets/basic");


	mage::Timer timer;
	int frames = 0;

	while (!display.isClosed())
	{
		display.clear(0, 0, 0);
		// Render
		shader->enable();


		renderer.draw(testMesh);


		shader->disable();



		display.update();

		if (timer.elapsed() >= 1.0)
		{
			// printf("FPS: %i\n", frames);
			mage::println(mage::console::BRIGHT_RED, "FPS: {}", frames);
			// fmt::print(fg(fmt::terminal_color::bright_cyan), "FPS: {}\n", frames);
			frames = 0;
			timer.reset();
		}
		else frames++;
	}

	if(testMesh) delete testMesh;
	if(shader) delete shader;
	return 0;
}
