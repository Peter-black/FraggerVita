#ifndef _TARGET_H
#define _TARGET_H

#include "entity.h"

class Target : public Entity{
	public:
		Target();
		Target(b2Vec2 position, b2World* world);
		~Target();

	protected:	
		
	private:
};

#endif