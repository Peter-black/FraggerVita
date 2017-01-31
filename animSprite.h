#ifndef _ANIM_SPRITE_H
#define _ANIM_SPRITE_H

#include <graphics\sprite.h>

class AnimSprite : public abfw::Sprite{
	public:
		AnimSprite();
		AnimSprite(float width, float height, abfw::Texture* tex, int frames, int framespeed);
		~AnimSprite();

		void update();
		void start(abfw::Vector3 position, float rot);
		inline bool isActive(){return active;};

	protected:	
		void reset();
		int frame, maxFrames;
		int time, speed;
		float uvSize;
		bool active;

};

#endif