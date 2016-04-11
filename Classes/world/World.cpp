#include "World.h"
#include "Debug.h"
//Initialize the pointer to be null;
World * World::s_instance = NULL;

World::World() {
	
}

World* World::initWorld(Node* backgroundLayer, Node* actionLayer) {
	
	this->backgroundNode = backgroundLayer;
	this->actionNode = actionLayer;
	CCLOG("Created the world");
	//initialize the Pools
	initSpriteCache();
	//initialize all the game objects/entities
	initPools();
	initPlayers();
	//initialize all the entity runners
	return this;
}
World* World::initSpriteCache() {
	//this will be move to lua
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Assets/spritesheet/basic_zombie/BasicZombieSheet.plist", "Assets/spritesheet/basic_zombie/BasicZombieSheet.png");
	
	
	return this;
}

void World::update(float delta) {
	std::list<EntityRunner*>::iterator runnerIt=runnerList.begin();
	while (runnerIt != runnerList.end()) {
		((EntityRunner*)(*runnerIt))->update(delta);
	}
}

World* World::initPools() {
	playerPool = new ObjectPool<Player>();
	zombiePool = new ObjectPool<Zombie>();
	itemPool = new ObjectPool<Item>();
	//init all component pools

	return this;
}
World* World::initPlayers() {
	//create the player and its zombies
	//create Main character
	swiss = playerPool->New();
	swiss->init();
	this->playerList.push_back(swiss);
	//create other players
	return this;
}

void World::destroy() {
	//delete all object
	//if an object is from a pool, use pool.Delete(object);
}