#include "engine.h"
#include <iostream>

int main(int argc, char** argv)
{
	Display display("Game Engine", 1920, 1080);

	Timer timer;
	int frames = 0;

	while (!display.isClosed())
	{
		display.clear(255, 0, 0);

		display.update();

		if (timer.elapsed() >= 1.0)
		{
			printf("FPS: %i\n", frames);
			frames = 0;
			timer.reset();
		}
		else frames++;
	}

	return 0;
}