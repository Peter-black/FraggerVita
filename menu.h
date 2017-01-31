#ifndef _MENU_H
#define _MENU_H
#include <graphics/sprite.h>
#include <graphics/font.h>
#include <string>

class SpriteApplication;

class Menu{

public:
	enum Screen{SPLASH, TITLE, OPTIONS, GAME, WIN, LOSE};
	enum Input{UP, DOWN, SELECT, BACK};

	Menu();
	Menu(SpriteApplication* spriteApp);
	~Menu();
	void input(Input input);
	void update();
	void changeScreen(Screen menu);
	void doAction(int selection);
	inline Screen getCurrScreen(){return currScreen;};
	inline bool getInitGame(){return initGame;};
	abfw::Sprite getBGSprite();
	inline std::string* getOptionText(){return optionText;};
	inline int getMaxSelections(){return maxSelections;};
	inline int getCurrSelection(){return selection;};
	inline bool getDifficulty(){return difficulty;};
	inline bool getVolume(){return volume;};
	inline void setInitGame(bool flag){initGame = flag;};

private:
	abfw::Sprite splashSprite;
	abfw::Sprite menuSprite;
	abfw::Sprite backgroundSprite;
	abfw::Sprite winSprite;	
	abfw::Sprite loseSprite;
	std::string optionText[3];
	int selection, maxSelections;
	Screen currScreen;
	bool initGame;
	bool difficulty;
	bool volume;
	bool reset;
	int splashTimer;
};

#endif