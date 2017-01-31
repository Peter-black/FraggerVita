#ifndef _SPRITE_APPLICATION_H
#define _SPRITE_APPLICATION_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <graphics/font.h>
#include <audio/vita/audio_manager_vita.h>
#include <assets/png_loader.h>
#include <input/sony_controller_input_manager.h>
#include <Box2D/Box2D.h>
#include "menu.h"
#include "ui.h"
#include "level.h"
#include "animSprite.h"

namespace abfw
{
	class Platform;
	class Texture;
}

class SpriteApplication : public abfw::Application
{
public:
	SpriteApplication(abfw::Platform& platform);
	~SpriteApplication();
	void Init();
	void GameInit();
	void Reset();
	void CleanUp();
	bool Update(float ticks);
	void Render();
	abfw::Texture* createTexture(char *filename);

private:
	abfw::SonyControllerInputManager* controller_manager_;
	abfw::Font font_;
	abfw::AudioManagerVita* audio_manager_;
	abfw::PNGLoader png_loader_;
	abfw::SpriteRenderer* sprite_renderer_;
	float frame_rate_;

	Menu menu;
	UI ui;
	bool buttonCheck;//Stops the menu from automatically throwing the frag
	AnimSprite player_;
	AnimSprite explosion_;
	AnimSprite frag_reset_;

	b2World* world_;
	b2Body* ground_body_;

	int32 throw_sound,
		  cancel_sound,
		  bounce_sound,
		  explode_sound,
		  outofbounds_sound,
		  win_sound,
		  retry_sound,
		  menu_change_sound,
		  menu_select_sound;

	float charge;
	Level level;

};

#endif // _SPRITE_APPLICATION_H