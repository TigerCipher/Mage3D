#include "engine.h"
#include <GL/glew.h>

int main(int argc, char** argv)
{
	std::cout << "Hello world!" << std::endl;
	int error = glewInit();
	std::cout << error << std::endl;
	return 0;
}