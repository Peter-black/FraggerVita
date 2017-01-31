#include "sprite_application.h"
#include <graphics/sprite_renderer.h>
#include <system/platform.h>
#include <graphics/image_data.h>
#include <graphics/texture.h>
#include <iostream>
#include "box2d_helpers.h"
#include <math.h>


SpriteApplication::SpriteApplication(abfw::Platform& platform) :
	abfw::Application(platform),
	sprite_renderer_(NULL)
{
}

SpriteApplication::~SpriteApplication()
{
}

void SpriteApplication::Init()
{
	// load the font to draw the on-screen text
	bool font_loaded = font_.Load("comic_sans", platform_);
	if(!font_loaded)
	{
		std::cout << "Font failed to load." << std::endl;
		exit(-1);
	}

	menu = Menu(this);
	menu.changeScreen(Menu::SPLASH);

	// create a sprite renderer to draw the sprites
	sprite_renderer_ = platform_.CreateSpriteRenderer();

	audio_manager_ = new abfw::AudioManagerVita;

	// create input manager to read sony game controllers
	controller_manager_ = platform_.CreateSonyControllerInputManager();

	audio_manager_->LoadMusic("music.wav", platform_);
	audio_manager_->PlayMusic();

	// create the Box2D physics world
	b2Vec2 gravity(0.0f, -10.0f);
	world_ = new b2World(gravity);

	ui = UI(this);
	//load in the level bodys
	level.loadLevel(0, world_, this);

	//Load in sprites
	player_ = AnimSprite(80, 104, createTexture("fragger8.png"), 2, 6);
	player_.set_position(abfw::Vector3(110, 465, 0));
	explosion_ = AnimSprite(168, 160, createTexture("explosion8.png"), 8, 5);
	frag_reset_ = AnimSprite(24, 32, createTexture("fragreset8.png"), 6, 2);

	//load in sounds
	throw_sound = audio_manager_->LoadSample("fragThrow.wav", platform_);
	cancel_sound = audio_manager_->LoadSample("fragCancel.wav", platform_);
	bounce_sound = audio_manager_->LoadSample("fragBounce.wav", platform_);
	explode_sound = audio_manager_->LoadSample("fragExplode.wav", platform_);
	outofbounds_sound = audio_manager_->LoadSample("fragOob.wav", platform_);
	win_sound = audio_manager_->LoadSample("levelComplete.wav", platform_);
	retry_sound = audio_manager_->LoadSample("levelRetry.wav", platform_);
	menu_select_sound = audio_manager_->LoadSample("menuSelect.wav", platform_);
}

void SpriteApplication::GameInit(){
	level.reset(0);//transforms all the bodys to initial spots
	buttonCheck = false;//Resets the check to stop throwing a frag straight away
	
	ui.resetScore();
	charge = 0;
}

void SpriteApplication::CleanUp()
{
	// free up input manager
	delete controller_manager_;
	controller_manager_ = NULL;
	
	delete world_;
	world_ = NULL;

	// free up the sprite renderer
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete audio_manager_;
	audio_manager_ = NULL;

	audio_manager_->UnloadMusic();
	audio_manager_->UnloadAllSamples();

}

bool SpriteApplication::Update(float ticks)
{
	//Resets the game state
	if(menu.getInitGame() == true){
		if(retry_sound != -1)audio_manager_->PlaySample(retry_sound, false);

		GameInit();

		if(menu.getDifficulty())ui.setFragNum(3);//hard mode
		else ui.setFragNum(6);//easy mode

		menu.setInitGame(false);

		return true;
	}

	//Mute/unmute volume
	if(menu.getVolume()){
		audio_manager_->SetMasterVolume(0.999f);//Framework uses <1 and not <=1 so 1 doesnt work.
	}else{
		audio_manager_->SetMasterVolume(0);
	}

	// calculate the frame rate
	frame_rate_ = 1.0f / ticks;
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	//Input
	if(controller_manager_){

		controller_manager_->Update();
		const abfw::SonyController* controller = controller_manager_->GetController(0);

		if(controller){
			//ingame input
			if(menu.getCurrScreen() == Menu::GAME){
				//get analog input
				float lx = controller->left_stick_x_axis();
				float ly = controller->left_stick_y_axis();

				float angleL = atan2(lx, ly);

				if(lx == 0 && ly == 0)angleL = DEGREES_RADIANS(135);

				ui.setArrowAngle(-angleL+DEGREES_RADIANS(180));

				if(!level.getFrag()->getTossed()){//check if frag is not thrown
					if(ui.getFragNum() > 0){//check if there are any frags left

						if(controller->buttons_down() & ABFW_SONY_CTRL_CROSS){
							if(buttonCheck){//stops auto button pressing coming out of menus
								charge+=0.01f;
								if(charge >= 0.6)charge = 0.6;
							}
						}

						if(controller->buttons_released() & ABFW_SONY_CTRL_CROSS){
							if(buttonCheck){
								if(throw_sound != -1)audio_manager_->PlaySample(throw_sound, false);
								level.getFrag()->toss(-angleL-DEGREES_RADIANS(90), charge);
								player_.start(abfw::Vector3(110, 465, 0), 0);//start player throw animation
								ui.setFragNum(ui.getFragNum()-1);
							}
							buttonCheck = true;
							charge = 0;//reset power
						}

					}//end checking if any frags left
				}else{//check if the frag is thrown
					if(controller->buttons_pressed() & ABFW_SONY_CTRL_TRIANGLE){
						if(cancel_sound != -1)audio_manager_->PlaySample(cancel_sound, false);
						level.getFrag()->reset();
						//play frag reset animation at the current position and rotation
						frag_reset_.start(abfw::Vector3(level.getFrag()->position().x, level.getFrag()->position().y, 0) , level.getFrag()->rotation());
						charge = 0;
					}
				}
			}else{//end of Menu::GAME ( now doing menu inputs )

				if(controller->buttons_pressed() & ABFW_SONY_CTRL_UP){
					menu.input(Menu::UP);
				}
				if(controller->buttons_pressed() & ABFW_SONY_CTRL_DOWN){
					menu.input(Menu::DOWN);
				}

				if(controller->buttons_pressed() & ABFW_SONY_CTRL_CROSS){
					if(menu_select_sound != -1)audio_manager_->PlaySample(menu_select_sound, false);
					menu.input(Menu::SELECT);
					return true;
				}

				if(controller->buttons_pressed() & ABFW_SONY_CTRL_TRIANGLE){
					menu.input(Menu::BACK);
				}
			}//end of menu inputs
		}

	}//end of input

	//Game logic
	if(menu.getCurrScreen() == Menu::GAME){
		//update physics
		world_->Step(timeStep, velocityIterations, positionIterations);

		//If frag falls off the left of the screen reset it
		if(level.getFrag()->position().x < 0){
			if(outofbounds_sound != -1)audio_manager_->PlaySample(outofbounds_sound, false);
			level.getFrag()->reset();
		}

		//Explode the frag
		if(level.getFrag()->getFuseTimer() <= 0){
			if(explode_sound != -1)audio_manager_->PlaySample(explode_sound, false);
			explosion_.start(abfw::Vector3(level.getFrag()->position().x, level.getFrag()->position().y, 0) , level.getFrag()->rotation());

			//Explodes frag and loops through how many targets it hit
			for(int i = 0; i <= level.getFrag()->explode(level.getFrag()->getBody()->GetPosition(), world_); i++){
				ui.addScore(1000);
			}
		}

		//check win/lose conditions (dont change the screen until the pretty animations are over)
		if(ui.getScore() >= 3000 && !explosion_.isActive()){
			if(win_sound != -1)audio_manager_->PlaySample(win_sound, false);
			menu.changeScreen(Menu::WIN);
		}else if(ui.getFragNum() <= 0 && !level.getFrag()->getTossed() && !explosion_.isActive()){
			menu.changeScreen(Menu::LOSE);
		}

		//Collision resoloution
		b2Contact* contact = world_->GetContactList();
		int contact_count = world_->GetContactCount();

		//these stop it constantly colliding
		b2Body* dupeTestA;
		b2Body* dupeTestB;
		bool touched = false;

		for(int curr_contact = 0; curr_contact < contact_count; curr_contact++){

			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			//If the scenery hits off each other play a bounce noise
			if(contact->IsTouching()){
				if(abs(bodyA->GetLinearVelocity().x) >= 0.2f || abs(bodyA->GetLinearVelocity().y) >= 0.2f){
					if(bounce_sound != -1)audio_manager_->PlaySample(bounce_sound, false);
				}
				if(abs(bodyB->GetLinearVelocity().x) >= 0.2f || abs(bodyB->GetLinearVelocity().y) >= 0.2f){
					if(bounce_sound != -1)audio_manager_->PlaySample(bounce_sound, false);
				}
			}

			//If a frag hits a target
			if((int)bodyA->GetUserData() == Entity::FRAG){
				if((int)bodyB->GetUserData() == Entity::TARGET && bodyB != dupeTestB){
					if(contact->IsTouching() && !touched){//Proper hitting not AABB
						//It needs to hit it with a minimum speed so that it doesnt count when resting
						if(abs(bodyA->GetLinearVelocity().x) >= 0.3f || abs(bodyA->GetLinearVelocity().y) >= 0.3f)ui.addScore(10);
						touched = true;
					}else touched = false;
				}
			}
			if((int)bodyB->GetUserData() == Entity::FRAG){
				if((int)bodyA->GetUserData() == Entity::TARGET && bodyA != dupeTestA){
					if(contact->IsTouching() && !touched){
						if(abs(bodyB->GetLinearVelocity().x) >= 0.5f || abs(bodyB->GetLinearVelocity().y) >= 0.5f)ui.addScore(10);
						touched = true;
					}else touched = false;
				}
			}

			dupeTestA = bodyA;
			dupeTestB = bodyB;
			
			contact->GetNext();
		}

		//Updating other classes
		if(explosion_.isActive())explosion_.update();
		if(frag_reset_.isActive())frag_reset_.update();
		if(player_.isActive())player_.update();
		level.update();
		ui.update(charge);
	}
	menu.update();
	return true;
}

void SpriteApplication::Render()
{
	// set up sprite renderer for drawing
	sprite_renderer_->Begin();

	//BACKGROUND
	sprite_renderer_->DrawSprite(menu.getBGSprite());

	//Ingame rendering
	if(menu.getCurrScreen() == Menu::GAME){
		//LEVEL
		sprite_renderer_->DrawSprite(*level.getFrag());
		for(int i = 0; i < level.getNumWalls(); i++) sprite_renderer_->DrawSprite(level.getWallArray()[i]);
		for(int i = 0; i < level.getNumEnts(); i++) sprite_renderer_->DrawSprite(level.getEntityArray()[i]);
		
		//UI
		for(int i = 0; i < 6; i ++) sprite_renderer_->DrawSprite(ui.getSprites()[i]);
		for(int i = 0; i < 3; i ++) font_.RenderText(sprite_renderer_, abfw::Vector3(25.0f + i*200, 0.0f, -0.9f), 0.75f, 0xffffffff, abfw::TJ_LEFT, ui.getText()[i].c_str());
		
		//Anim sprites
		if(frag_reset_.isActive()) sprite_renderer_->DrawSprite(frag_reset_);
		sprite_renderer_->DrawSprite(player_);
		if(explosion_.isActive()) sprite_renderer_->DrawSprite(explosion_);
	}

	//MENU OPTIONS
	for(int i = 0; i <= menu.getMaxSelections(); i++){
		if(menu.getMaxSelections() == 0)break;
		UInt32 colour;
		if(i == menu.getCurrSelection())colour = 0xFF000000;
		else colour = 0xFF999999;
		font_.RenderText(sprite_renderer_, abfw::Vector3(600.0f, 325.0f+i*60, -0.9f), 1.75f, colour, abfw::TJ_LEFT, menu.getOptionText()[i].c_str());
	}
	//Score value in win/lose screens
	if(menu.getCurrScreen() == Menu::WIN || menu.getCurrScreen() == Menu::LOSE){
		std::string s;
		std::stringstream out;
		out << ui.getScore();
		s = "Score: " + out.str();

		font_.RenderText(sprite_renderer_, abfw::Vector3(700.0f, 125.0f, -0.9f), 1.75f, 0xFF0000FF, abfw::TJ_LEFT, s.c_str());
	}

	//FPS counter
	font_.RenderText(sprite_renderer_, abfw::Vector3(850.0f, 0.0f, -0.9f), 1.0f, 0xff00ffff, abfw::TJ_LEFT, "FPS: %.1f", frame_rate_);

	sprite_renderer_->End();
}

abfw::Texture* SpriteApplication::createTexture(char *filename){
	abfw::ImageData image_data_;
	png_loader_.Load(filename, platform_, image_data_);
	abfw::Texture* tex_ = platform_.CreateTexture(image_data_);

	return tex_;
}