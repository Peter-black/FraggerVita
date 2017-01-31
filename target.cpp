#include "target.h"

Target::Target(){
}

Target::~Target(){
}

Target::Target(b2Vec2 position, b2World* world)
	:Entity(position, world){
		
		b2PolygonShape shapeBody;
		shapeBody.SetAsBox(ABFW_BOX2D_SIZE(18), ABFW_BOX2D_SIZE(48));

		b2CircleShape shapeBelly;
		shapeBelly.m_radius = 0.25f;

		//Main rectangular body
		b2FixtureDef fixtureBody;
		fixtureBody.shape = &shapeBody;
		fixtureBody.density = 0.75f;
		fixtureBody.friction = 0.7f;
		fixtureBody.restitution = 0.2f;

		//Circle in middle for belly physics
		b2FixtureDef fixtureBelly;
		fixtureBelly.shape = &shapeBelly;
		fixtureBelly.density = 0.75f;
		fixtureBelly.friction = 0.7f;
		fixtureBelly.restitution = 0.2f;

		body_->CreateFixture(&fixtureBody);
		body_->CreateFixture(&fixtureBelly);

		body_->SetUserData((void*)TARGET);
}
