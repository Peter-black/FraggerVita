#ifndef _UI_H
#define _UI_H

#include <graphics/sprite.h>
#include <graphics/font.h>
#include <string>
#include <sstream>

class SpriteApplication;

class UI{

public:
	UI();
	UI(SpriteApplication* spriteApp);
	~UI();
	void update(float charge);
	inline void addScore(int amount){score+=amount;};
	inline void setArrowAngle(float angle){sprites[0].set_rotation(angle);};
	inline void setFragNum(int number){numFrags = number;};
	inline int getFragNum(){return numFrags;};
	inline std::string* getText(){return text;};
	abfw::Sprite* getSprites(){return sprites;};
	inline int getScore(){return score;};
	inline void resetScore(){score = 0;};

private:
	std::string toString(int num);
	
	abfw::Sprite sprites[6];
	std::string text[3];
	bool cloudBounceDir;
	float cloudRotation;
	int numFrags,score;
};

#endif