#ifndef _WALL_H
#define _WALL_H

#include <graphics\sprite.h>
#include <Box2D/Box2D.h>
#include "box2d_helpers.h"

class Wall : public abfw::Sprite{
	public:
		enum ObjectType{FRAG, TARGET, CRATE, WALL, DEAD};
		Wall();
		Wall(b2Vec2 position, b2Vec2 size, b2World* world);
		~Wall();

		inline b2Body* getBody(){return body_;};

	protected:	
		b2Body* body_;

};

#endif