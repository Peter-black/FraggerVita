#include "sprite_application.h"
#include <system/vita/platform_vita.h>

unsigned int sceLibcHeapSize = 128*1024*1024;	// Sets up the heap area size as 128MiB.

int main()
{
	abfw::PlatformVita platform_vita;
	SpriteApplication myApplication(platform_vita);

	myApplication.Run();
	return 0;
}