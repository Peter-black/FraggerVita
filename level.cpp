#include "level.h"
#include "box2d_helpers.h"
#include "sprite_application.h"

Level::Level(){
	numWalls = 0;
	numEnts = 0;
}

Level::~Level(){

}

void Level::loadLevel(int level, b2World* world, SpriteApplication* spriteApp){

	switch(level){
	case 0:
		throwPoint.Set(ABFW_BOX2D_POS_X(100),ABFW_BOX2D_POS_Y(465));

		//Scenery
		walls[0] = Wall(b2Vec2(ABFW_BOX2D_POS_X(960*0.5), ABFW_BOX2D_POS_Y(544-20)), 
				  b2Vec2(ABFW_BOX2D_SIZE(960),ABFW_BOX2D_SIZE(40.0f)), world);
		walls[0].set_texture(spriteApp->createTexture("ground8.png"));

		walls[1] = Wall(b2Vec2(ABFW_BOX2D_POS_X(960-20), ABFW_BOX2D_POS_Y(252)), 
					  b2Vec2(ABFW_BOX2D_SIZE(40),ABFW_BOX2D_SIZE(504)), world);
		walls[1].set_texture(spriteApp->createTexture("wall8.png"));

		walls[2] = Wall(b2Vec2(ABFW_BOX2D_POS_X(675), ABFW_BOX2D_POS_Y(385)), 
					  b2Vec2(ABFW_BOX2D_SIZE(312),ABFW_BOX2D_SIZE(24)), world);
		walls[2].set_texture(spriteApp->createTexture("horizwall8.png"));

		walls[3] = Wall(b2Vec2(ABFW_BOX2D_POS_X(540), ABFW_BOX2D_POS_Y(448)), 
					  b2Vec2(ABFW_BOX2D_SIZE(24),ABFW_BOX2D_SIZE(112)), world);
		walls[3].set_texture(spriteApp->createTexture("leftwall8.png"));

		numWalls = 4;
	
		//Entities
		frag = Frag(throwPoint, world);
		frag.set_texture(spriteApp->createTexture("frag8.png"));
		frag.set_width(18.0f);
		frag.set_height(25.0f);

		entities[0] = Target(b2Vec2 (1.0f, 1.0f), world);
		entities[0].set_texture(spriteApp->createTexture("badguy8.png"));
		entities[0].set_width(48.0f);
		entities[0].set_height(96.0f);

		entities[1] = Target(b2Vec2 (3.0f, 1.0f), world);
		entities[1].set_texture(spriteApp->createTexture("badguy8.png"));
		entities[1].set_width(48.0f);
		entities[1].set_height(96.0f);

		entities[2] = Target(b2Vec2 (1.5f, 3.0f), world);
		entities[2].set_texture(spriteApp->createTexture("badguy8.png"));
		entities[2].set_width(48.0f);
		entities[2].set_height(96.0f);

		entities[3] = Crate(b2Vec2 (0.7f, 2.0f), world);
		entities[3].set_texture(spriteApp->createTexture("crate8.png"));
		entities[3].set_width(45.0f);
		entities[3].set_height(45.0f);

		entities[4] = Crate(b2Vec2 (0.7f, 3.0f), world);
		entities[4].set_texture(spriteApp->createTexture("crate8.png"));
		entities[4].set_width(45.0f);
		entities[4].set_height(45.0f);

		numEnts = 5;

		break;

	case 1:

		break;
	}
}

void Level::reset(int level){

	switch(level){
	case 0:
		//Scenery
		walls[0].getBody()->SetTransform(b2Vec2(ABFW_BOX2D_POS_X(960*0.5), ABFW_BOX2D_POS_Y(544-20)), 0);
		walls[1].getBody()->SetTransform(b2Vec2(ABFW_BOX2D_POS_X(960-20), ABFW_BOX2D_POS_Y(252)), 0);
		walls[2].getBody()->SetTransform(b2Vec2(ABFW_BOX2D_POS_X(675), ABFW_BOX2D_POS_Y(385)), 0);
		walls[3].getBody()->SetTransform(b2Vec2(ABFW_BOX2D_POS_X(540), ABFW_BOX2D_POS_Y(448)), 0);

		//Entities
		frag.getBody()->SetTransform(throwPoint, 0);

		entities[0].getBody()->SetTransform(b2Vec2 (1.0f, 1.0f), 0);
		entities[1].getBody()->SetTransform(b2Vec2 (3.0f, 1.0f), 0);
		entities[2].getBody()->SetTransform(b2Vec2 (1.5f, 3.0f), 0);
		entities[3].getBody()->SetTransform(b2Vec2 (0.7f, 2.0f), 0);
		entities[4].getBody()->SetTransform(b2Vec2 (0.7f, 3.0f), 0);

		for(int i = 0; i < numEnts; i++){
			entities[i].getBody()->SetLinearVelocity(b2Vec2(0,0));
			entities[i].getBody()->SetAngularVelocity(0);
		}

		break;

	case 1:

		break;
	}
}
	
void Level::update(){
	frag.update();
	for(int i = 0; i < numEnts; i++){
		entities[i].update();
	}
}
