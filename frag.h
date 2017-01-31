#ifndef _FRAG_H
#define _FRAG_H

#include "entity.h"

class Frag : public Entity{
	public:
		Frag();
		Frag(b2Vec2 position, b2World* world);
		~Frag();
		void update();
		void toss(float angle, float charge);
		int explode(b2Vec2 position, b2World* world);
		void reset();
		inline void addFuseTime(int time){fuseTimer += time;};
		inline int getFuseTimer(){return fuseTimer;};
		inline int getMaxFuseTime(){return maxFuseTime;};
		inline bool getTossed(){return tossed;};

	protected:	
		
	private:
		void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);

		bool tossed;
		int fuseTimer, maxFuseTime;
};

#endif