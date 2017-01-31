#include "ui.h"
#include "sprite_application.h"

UI::UI(){
}

UI::UI(SpriteApplication* spriteApp){
	//Arrow
	sprites[0].set_width(56.0f);
	sprites[0].set_height(352.0f);
	sprites[0].set_position(abfw::Vector3(65, 445, 0));
	sprites[0].set_texture(spriteApp->createTexture("arrow8.png"));
	sprites[0].set_colour(0x55FFFFFF);
	//PowerBarCover
	sprites[1].set_width(104.0f);
	sprites[1].set_height(32.0f);
	sprites[1].set_position(abfw::Vector3(80, 50, 0));
	sprites[1].set_texture(spriteApp->createTexture("power8.png"));
	//PowerBar
	sprites[2].set_width(0.0f);
	sprites[2].set_height(26.0f);
	sprites[2].set_position(abfw::Vector3(81, 50, 0));
	sprites[2].set_colour(0x9900FF00);
	//Cloud1
	sprites[3].set_width(128.0f);
	sprites[3].set_height(48.0f);
	sprites[3].set_position(abfw::Vector3(200, 100, 0));
	sprites[3].set_texture(spriteApp->createTexture("cloudOne8.png"));
	//Cloud2
	sprites[4].set_width(88.0f);
	sprites[4].set_height(40.0f);
	sprites[4].set_position(abfw::Vector3(450, 150, 0));
	sprites[4].set_texture(spriteApp->createTexture("cloudTwo8.png"));
	//Cloud3
	sprites[5].set_width(128.0f);
	sprites[5].set_height(48.0f);
	sprites[5].set_position(abfw::Vector3(680, 200, 0));
	sprites[5].set_texture(spriteApp->createTexture("cloudOne8.png"));
	
	cloudBounceDir = false;
	cloudRotation = 0;

	numFrags = 0;
	score = 0;

	text[0] = "Frags: " + toString(numFrags);
	text[1] = "Score: " + toString(score);
	text[2] = "Level: 1";
}

UI::~UI(){
}

void UI::update(float charge){
	text[0] = "Frags: " + toString(numFrags);
	text[1] = "Score: " + toString(score);

	//makes the clouds bounce left and right
	for(int i = 3; i < 6; i++){//cloud sprite indexs
		if(cloudBounceDir){
			if(cloudRotation >= 0.30f){
				cloudBounceDir = !cloudBounceDir;
			}
			cloudRotation += 0.005f;
			sprites[i].set_rotation(cloudRotation);
		}else{
			if(cloudRotation <= -0.30f){
				cloudBounceDir = !cloudBounceDir;
			}
			cloudRotation -= 0.005f;
			sprites[i].set_rotation(cloudRotation);
		}
	}
	//powerbar
	sprites[2].set_width(charge*166);
	sprites[2].set_position(abfw::Vector3(30+charge*83, 50, 0));
}

std::string UI::toString(int num){
	std::string s;
	std::stringstream out;
	out << num;
	s = out.str();
	return s;
}
