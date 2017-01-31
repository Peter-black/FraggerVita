#include "animSprite.h"

AnimSprite::AnimSprite(){

}

AnimSprite::AnimSprite(float width, float height, abfw::Texture* tex, int frames, int framespeed){
	set_position(abfw::Vector3(0,0,0));
	set_width(width);
	set_height(height);
	set_texture(tex);
	maxFrames = frames;
	frame = 0;
	speed = framespeed;
	time = 0;
	active = false;
	uvSize = (float)1/maxFrames;
	set_uv_width(uvSize);
}

AnimSprite::~AnimSprite(){

}

void AnimSprite::update(){
	
	time++;

	if(time >= speed){
		frame++;
		time = 0;
	}

	set_uv_position(abfw::Vector2((float)frame*uvSize, 1));

	if(frame >= maxFrames){
		active = false; 
		reset();
		return;
	}

}

void AnimSprite::start(abfw::Vector3 position, float rot){
	active = true;
	set_position(position);
	set_rotation(rot);
}

void AnimSprite::reset(){
	frame = 0;
	time = 0;
	active = false;
}