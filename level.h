#ifndef _LEVEL_H
#define _LEVEL_H

#include <Box2D/Box2D.h>
#include "wall.h"
#include "entity.h"
#include "frag.h"
#include "target.h"
#include "crate.h"

class SpriteApplication;

class Level{
public:
	enum Type{FRAG, TARGET, CRATE, WALL};

	Level();
	~Level();
	void loadLevel(int level, b2World* world, SpriteApplication* spriteApp);
	void update();
	void reset(int level);
	inline Frag* getFrag(){return &frag;};
	inline Wall* getWallArray(){return walls;};
	inline Entity* getEntityArray(){return entities;};
	inline b2Vec2 getThrowPoint(){return throwPoint;};
	inline int getNumWalls(){return numWalls;};
	inline int getNumEnts(){return numEnts;};

private:
	b2Vec2 throwPoint;//point the grenade is thrown from
	Wall walls[5];
	int numWalls;
	Entity entities[10];
	int numEnts;
	Frag frag;

};

#endif 