#include "crate.h"

Crate::Crate(){
}

Crate::~Crate(){
}

Crate::Crate(b2Vec2 position, b2World* world)
	:Entity(position, world){
		
		b2PolygonShape shape;
		shape.SetAsBox(ABFW_BOX2D_SIZE(45/2), ABFW_BOX2D_SIZE(45/2));

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 0.75f;
		fixtureDef.friction = 0.7f;
		fixtureDef.restitution = 0.2f;
		body_->CreateFixture(&fixtureDef);

		body_->SetUserData((void*)CRATE);
}
