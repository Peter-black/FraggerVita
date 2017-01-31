#include <Box2D/Box2D.h>
 
class RayCastClosestCallback : public b2RayCastCallback
{
public:
    RayCastClosestCallback(){
    }
 
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {        
        fixture_ = fixture;        
		body_ = fixture->GetBody();
        point_ = point;        
        normal_ = normal;        
        fraction_ = fraction;        
        return fraction;     
    }    
 
    b2Fixture* fixture_;    
	b2Body* body_;
    b2Vec2 point_;    
    b2Vec2 normal_;    
    float32 fraction_;
 
};