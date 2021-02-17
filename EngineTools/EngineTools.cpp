
#include <Mage3DX/TextureProcessor.h>
#include <Mage3DX/Util.h>

#include "Mage3DX/MageException.h"
#include <Mage3DX/Display.h>
#include <Mage3DX/Settings.h>

int main(int argc, char** argv)
{
	Log::init();


	//TextureProcessor::flipYNormalMap("assets\\textures\\brickwall_normal.jpg", "testtools.jpg");
	try{
		Settings::load("settings.ini");
		Display display(1280, 720, "Title", false);
		if(argc < 2)
		{
			fmt::print("Engine tools are useless without program arguments\n");
			return -1;
		}
		std::string cmd = argv[1];
		if(cmd == "--flipY" || cmd == "--flipy" || cmd == "-fy")
		{
			if(argc < 3)
			{
				fmt::print("FlipY expects at least one argument\n");
				return -1;
			}
			std::string src = argv[2];
			std::string dest = src;
			if(argc >= 4)
				dest = argv[3];
			TextureProcessor::flipYNormalMap(src, dest);
			
		}
	}catch(const MageException& e)
	{
		LOG_CRITICAL("Caught a Mage3D exception of type [{}]. Info:\n{}\n\nStacktrace:\n{}", e.getType(), e.what(), e.getStacktrace());
		MessageBox(nullptr, e.what(), "Exception Caught!", MB_OK | MB_ICONEXCLAMATION);
	}

	return 0;
}
