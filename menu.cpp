#include "menu.h"
#include "sprite_application.h"

Menu::Menu(){
}

Menu::Menu(SpriteApplication* spriteApp){

	splashSprite.set_texture(spriteApp->createTexture("splash8.png"));
	splashSprite.set_position(abfw::Vector3(480, 272, 0));
	splashSprite.set_width(960);
	splashSprite.set_height(544);

	menuSprite.set_texture(spriteApp->createTexture("menu8.png"));
	menuSprite.set_position(abfw::Vector3(480, 272, 0));
	menuSprite.set_width(960);
	menuSprite.set_height(544);

	backgroundSprite.set_texture(spriteApp->createTexture("background8.png"));
	backgroundSprite.set_position(abfw::Vector3(480, 272, 0));
	backgroundSprite.set_width(960);
	backgroundSprite.set_height(544);

	winSprite.set_texture(spriteApp->createTexture("win8.png"));
	winSprite.set_position(abfw::Vector3(480, 272, 0));
	winSprite.set_width(960);
	winSprite.set_height(544);

	loseSprite.set_texture(spriteApp->createTexture("lose8.png"));
	loseSprite.set_position(abfw::Vector3(480, 272, 0));
	loseSprite.set_width(960);
	loseSprite.set_height(544);

	currScreen = SPLASH;
	selection = 0;
	maxSelections = 0;
	difficulty = false;
	volume = true;
	initGame = false;
	splashTimer = 0;
}

Menu::~Menu(){
}

void Menu::input(Input input){
	switch(input){
		case UP:
			selection--;
			if(selection <= 0)selection = 0;
			break;

		case DOWN:
			selection++;
			if(selection >= maxSelections)selection = maxSelections;
			break;

		case SELECT:
			doAction(selection);
			break;

		case BACK:
			if(currScreen == OPTIONS)changeScreen(TITLE);
			break;
	}
}

void Menu::update(){
	switch(currScreen){
		//make splash auto dissapear (No skipping it ofcourse)
		case SPLASH:splashTimer++;
			if(splashTimer >= 100)changeScreen(TITLE);
			break;
		case OPTIONS:
			optionText[0] = std::string("Difficulty: ") + std::string(difficulty ? "Hard" : "Easy");
			optionText[1] = std::string("Volume: ") + std::string(volume ? "On" : "Mute");
			break;
	}
}

void Menu::doAction(int selection){
	switch(currScreen){
		case TITLE:
			switch(selection){
				case 0:changeScreen(GAME);//Play
					break;
				case 1: changeScreen(OPTIONS);//Options menu
					break;
				case 2: exit(0);//Quit
					break;
			}
			break;
		
		case OPTIONS:
			switch(selection){
				case 0: difficulty = !difficulty;//change difficulty
					break;
				case 1: volume = !volume;//Mute audio
					break;
				case 2: changeScreen(TITLE);//return to title
					break;
			}
			break;

		case LOSE:
		case WIN:
			switch(selection){
				case 0: changeScreen(TITLE);//return to menu
					break;
				case 1:changeScreen(GAME);//replay
					break;
				case 2: exit(0);//QUIT
					break;
			}
			break;

		default://do nothing if in SPLASH or GAME screen
			break;
	}
}

void Menu::changeScreen(Screen menu){
	currScreen = menu;
	selection = 0;
	switch(currScreen){
		case GAME: initGame = true;
			maxSelections = 0;
			break;
		case TITLE:maxSelections = 2;
			optionText[0]= "Play";
			optionText[1]= "Options";
			optionText[2]= "Quit";
			break;
		case OPTIONS:maxSelections = 2;
			optionText[0]= "Difficulty:";
			optionText[1]= "Volume:";
			optionText[2]= "Return";
			break;
		case LOSE:
		case WIN:maxSelections = 2;
			optionText[0]= "Menu";
			optionText[1]= "Replay";
			optionText[2]= "Quit";
			break;
		default: maxSelections = 0;
	}
}

abfw::Sprite Menu::getBGSprite(){
	switch(currScreen){
		case SPLASH:return splashSprite;
		case LOSE:return loseSprite;
		case WIN:return winSprite;
		case GAME: return backgroundSprite;
		default: return menuSprite;
	}
}
