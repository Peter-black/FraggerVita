#ifndef _ENTITY_H
#define _ENTITY_H

#include <graphics\sprite.h>
#include <Box2D/Box2D.h>
#include "box2d_helpers.h"

class Entity : public abfw::Sprite{
	public:
		enum ObjectType{FRAG, TARGET, CRATE, WALL, DEAD};
		Entity();
		Entity(b2Vec2 position, b2World* world);
		~Entity();

		void update();
		inline b2Body* getBody(){return body_;};

	protected:	
		b2Body* body_;

};

#endif