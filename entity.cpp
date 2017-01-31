#include "entity.h"

Entity::Entity(){
}

Entity::Entity(b2Vec2 position, b2World* world){
	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body_def.position = position;
	body_ = world->CreateBody(&body_def);
}

void Entity::update(){
	set_position(abfw::Vector3(BOX2D_ABFW_POS_X(body_->GetPosition().x), BOX2D_ABFW_POS_Y(body_->GetPosition().y), 0.f));
	set_rotation(-(body_->GetAngle()));
}

Entity::~Entity(){
}