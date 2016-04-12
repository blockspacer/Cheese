#include "KineticRunner.h"
#include "world/World.h"
#include "component/ComponentCatalog.h"
#include "entity/Entity.h"
#include "component/pooled/KineticComp.h"
#include <vector>

KineticRunner::KineticRunner() {

}

void KineticRunner::update(float delta) {


	//I know that only Swiss will be affected by keyboard input directly, so I just get Swiss
	auto keyStatus = World::instance()->keyStatus;
	auto swiss = World::instance()->swiss;
	KineticComp* kineticComp = (KineticComp*)(swiss->components[COMP_CA::KINETIC_COMP]);
	kineticComp->step(swiss);

	//Swiss zombie moves
	auto swissZombieIt = swiss->zombies.begin();
	while (swissZombieIt != swiss->zombies.end()) {
		Zombie* z = (*swissZombieIt);
		KineticComp* kineticCompZ = (KineticComp*)(z->components[COMP_CA::KINETIC_COMP]);
		kineticCompZ->stepZ(z);
		kineticCompZ->setSpritePos((*swissZombieIt));
		if (kineticCompZ->pos.distance(kineticComp->pos) < 100.0f && !kineticComp->resting) {
			kineticCompZ->vel.set(kineticCompZ->maxSpeed*Vec2(kineticComp->vel)/kineticComp->maxSpeed);
		}
		else {
			float angle = kineticCompZ->pos.getAngle(Vec2(kineticComp->pos));
		//	float angle = kineticComp->pos.getAngle(Vec2(kineticCompZ->pos));
			CCLOG("angle is %f or %f", angle*180/3.1415, angle);
			kineticCompZ->vel.set(kineticCompZ->maxSpeed*sin(angle+(5*3.1415/4)), kineticCompZ->maxSpeed*cos((angle + 5 * 3.1415 / 4)));
		}
		swissZombieIt++;
	}



	//zombies
	auto strayZombie = World::instance()->strayZombie;
	auto zombieIt = strayZombie->zombies.begin();
	while (zombieIt != strayZombie->zombies.end()) {
		Zombie* z = (*zombieIt);
		KineticComp* kineticCompZ = (KineticComp*)(z->components[COMP_CA::KINETIC_COMP]);
		kineticCompZ->setSpritePos((*zombieIt));
		zombieIt++;
	}
	zombieIt = strayZombie->zombies.begin();
	while (zombieIt != strayZombie->zombies.end()) {
		Zombie* z = (*zombieIt);
		KineticComp* kineticCompZ = (KineticComp*)(z->components[COMP_CA::KINETIC_COMP]);
		if (kineticCompZ->pos.distance(kineticComp->pos) < 40.0f) {
			swiss->zombies.push_back(z);
			strayZombie->zombies.remove(z);
			CCLOG("SUCCESSFULLY CONVERTED A ZOMBIE");
			break;
		}
		zombieIt++;
	}



}