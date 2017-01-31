#include "wall.h"
#include "box2d_helpers.h"

Wall::Wall(){
}

Wall::Wall(b2Vec2 position, b2Vec2 size, b2World* world){
	b2BodyDef body_def;
	body_def.position = position;

	body_ = world->CreateBody(&body_def);

	b2PolygonShape ground_shape;
	ground_shape.SetAsBox(size.x/2, size.y/2);

	body_->CreateFixture(&ground_shape, 0.0f);

	body_->SetUserData((void*)WALL);

	set_width(BOX2D_ABFW_SIZE(size.x));
	set_height(BOX2D_ABFW_SIZE(size.y));
	set_position(abfw::Vector3(BOX2D_ABFW_POS_X(position.x), BOX2D_ABFW_POS_Y(position.y), 0));
}

Wall::~Wall(){
}