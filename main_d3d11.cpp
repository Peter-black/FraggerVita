#include "sprite_application.h"
#include <system/d3d11/platform_d3d11.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	abfw::PlatformD3D11 platform_d3d11(hInstance, 960, 544, false, true);
	SpriteApplication myApplication(platform_d3d11);

	myApplication.Run();
	return 0;
}