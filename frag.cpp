#include "frag.h"
#include "rayCastClosestCallback.h"
#include <math.h>

Frag::Frag(){
}

Frag::~Frag(){
}

Frag::Frag(b2Vec2 position, b2World* world)
	:Entity(position, world){
	b2CircleShape shape;
	shape.m_radius = 0.12f;
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 2.0f;
	fixtureDef.restitution = 0.3f;
	body_->SetBullet(true);
	body_->CreateFixture(&fixtureDef);
	body_->SetLinearDamping(0.25f);//friction substitute (friction doesnt work with box2d circles)

	body_->SetUserData((void*)FRAG);
	
	maxFuseTime = 200;//how long the frag can be active for
	fuseTimer = maxFuseTime;
	tossed = false;
}

void Frag::update(){
	if(!tossed){//keep the frag in the fragger's hand
		body_->SetTransform(b2Vec2(ABFW_BOX2D_POS_X(80),ABFW_BOX2D_POS_Y(445)), 0.0f);
		body_->SetLinearVelocity(b2Vec2(0,0));
		body_->SetAngularVelocity(0);
	}else{
		fuseTimer--;
	}
	set_position(abfw::Vector3(BOX2D_ABFW_POS_X(body_->GetPosition().x), BOX2D_ABFW_POS_Y(body_->GetPosition().y), 0.f));
	set_rotation(-(body_->GetAngle()));
}

void Frag::toss(float angle, float charge){//throws frag at specified angle
	fuseTimer = maxFuseTime;
	body_->ApplyLinearImpulse(b2Vec2(-charge*cos(angle)*1.5f,charge*sin(angle)+charge), body_->GetWorldCenter());
	body_->SetAngularVelocity(-5.0f);
	tossed = true;
}

int Frag::explode(b2Vec2 position, b2World* world){
	//perform a raycast to check for other fixtures around the frag
	//destroy the targets
	int numRays = 60;
	float blastRadius = 1.0f;
	float blastPower = 1.5f;
	int numTargetsHit = 0;
	
	for (int i = 0; i < numRays; i++) {
		float angle = DEGREES_RADIANS((i / (float)numRays) * 360);
		b2Vec2 rayDir( sinf(angle), cosf(angle) );
		b2Vec2 rayEnd = position + blastRadius * rayDir;
		
		//get the results of the raycast
		RayCastClosestCallback callback;
		world->RayCast(&callback, position, rayEnd);

		//get closest impact of ray
		if(callback.fraction_ < 1 && callback.fraction_ > 0){
			if((int)callback.body_->GetUserData() == TARGET){
				callback.body_->SetTransform(b2Vec2(-100,0),0);//kill target
				numTargetsHit++;
			}
			//explode!
			applyBlastImpulse(callback.body_, position, callback.point_, (blastPower / (float)numRays));
		}
	}
	//reset frag
	reset();
	return numTargetsHit;
}

void Frag::applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower) {
	//Applys appropriate impulse to bodys
	b2Vec2 blastDir = applyPoint - blastCenter;
	float distance = blastDir.Normalize();
	if(distance == 0)return;
	float invDistance = 1 / distance;
	float impulseMag = blastPower * invDistance * invDistance;
	body->ApplyLinearImpulse( impulseMag * blastDir, applyPoint );
 }

void Frag::reset(){
	fuseTimer = maxFuseTime;
	tossed = false;
	body_->SetLinearVelocity(b2Vec2(0,0));
	body_->SetAngularVelocity(0);
}
