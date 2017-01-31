#ifndef _CRATE_H
#define _CRATE_H

#include "entity.h"

class Crate : public Entity{
	public:
		Crate();
		Crate(b2Vec2 position, b2World* world);
		~Crate();

	protected:	
		
	private:
};

#endif