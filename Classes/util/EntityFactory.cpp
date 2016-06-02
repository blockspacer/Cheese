#include "EntityFactory.h"
#include "world/World.h"
#include "ObjectPool/ObjectPool.h"
#include "cocos2d.h"
#include "entity/zombie/ZombieStatCatalog.h"
#include "entity/player/PlayerCatalog.h"
#include "util/AttachmentFactory.h"

#include "util/ZombieStatHelper.h"
USING_NS_CC;


void EntityFactory::initEntity(Entity* entity, LuaTable& luaTable) {
	auto world = World::instance();
//	CCLOG("init entities");
	//loading all the data
	//basic components
#define luabool(st) luaTable.Get<bool>(st)
#define luastring(st) LuaDevice::instance()->getString(luaTable.Get<std::string>(st))
#define luaint(st) luaTable.Get<int>(st)
#define luafloat(st) luaTable.Get<float>(st)
#define luazombfunc(st) luaTable.Get<LuaFunction<LuaTable()>>(st)
#define newcomp(type, enum) world->getCompPool<type>(enum)->New()
#define addcomp(enum, comp) entity->components[enum] = comp

	entity->alliance = luaint("alliance");
	if (luabool("ChasingComp")) {
		//create chasing comp
//		CCLOG("creating chasing comp");
		entity->components[COMP_CA::CHASING_COMP] = world->commonComps[COMP_CA::CHASING_COMP];
	}
	if (luabool("SeparationComp")) {
		CCLOG("creating separation comp");
		entity->components[COMP_CA::SEPARATION_COMP] = world->commonComps[COMP_CA::SEPARATION_COMP];
	}
	if (luabool("FollowingComp")) {
//		CCLOG("creating followingComp");
		entity->components[COMP_CA::FOLLOWING_COMP] = world->commonComps[COMP_CA::FOLLOWING_COMP];
	}
	if (luabool("MeleeAttackComp")) {
//		CCLOG("creating meleeAttackComp");
		entity->components[COMP_CA::MELEE_ATTACK_COMP] = world->commonComps[COMP_CA::MELEE_ATTACK_COMP];
	}
	if (luabool("RangeAttackComp")) {
		//		CCLOG("creating rangeAttackComp");
		entity->components[COMP_CA::RANGE_ATTACK_COMP] = world->commonComps[COMP_CA::RANGE_ATTACK_COMP];
	}
	if (luabool("KeyboardComp")) {
//		CCLOG("creating keyboardComp");
		entity->components[COMP_CA::KEYBOARD_COMP] = world->commonComps[COMP_CA::KEYBOARD_COMP];
	}
	if (luabool("RecruitComp")) {
//		CCLOG("creating recruitComp");
		entity->components[COMP_CA::RECRUIT_COMP] = world->commonComps[COMP_CA::RECRUIT_COMP];
	}

	//pooled component
	if (luabool("ActionFlagComp")) {
//		CCLOG("creating actionflagComp");
		auto actionFlag = newcomp(ActionFlagComp, COMP_CA::ACTION_FLAG_COMP);
		actionFlag->init();
		//actionFlag->interval = luafloat("attackSpeed");
		actionFlag->interval = luafloat("actionFlag_interval");
		/*
		if (!luabool("belongToPlayer")) {
			actionFlag->interval = luafloat("WanderingCompInterval");
		}*/
		addcomp(COMP_CA::ACTION_FLAG_COMP, actionFlag);
		CCLOG("attack speed is %lf", actionFlag->interval);
		
	}
	if (luabool("AnimComp")) {
//		CCLOG("creating anim comp");
		auto anim = newcomp(AnimComp, COMP_CA::ANIM_COMP);
		anim->init();
		//anim->name=luastring("AnimCompName");
		//CCLOG("anim comp name is %s", anim->name.c_str());
		//anim->newAnimState = A_WALK_FORTH;
		anim->name = luastring("anim_name");
		anim->newAnimState = luastring("anim_newAnimState");
		anim->defaultAction = luastring("anim_defaultAction");

		addcomp(COMP_CA::ANIM_COMP, anim);
	}
	if (luabool("CombatComp")) {
		//CCLOG("creating combat comp");
		auto  combat = newcomp(CombatComp, COMP_CA::COMBAT_COMP);
		combat->init();
		combat->hp = luaint("combat_hp");
		combat->damage = luaint("combat_attack");
		//combat->attackSpeed = luafloat("attackSpeed");
		combat->isDead = false;
		combat->isDying = false;
		combat->pendingDmg = 0;
		combat->alliance = entity->alliance;//TODO: set alliance in lua file
		CCLOG("damage is %d", combat->damage);
		addcomp(COMP_CA::COMBAT_COMP, combat);
	}
	if (luabool("DomainComp")) {
//		CCLOG("creating domain comp");
		auto domain = newcomp(DomainComp, COMP_CA::DOMAIN_COMP);
		domain->init();
		domain->radius = (float)luaint("domain_radius");
		//CCLOG("Radius read is %d ", luaint("DomainCompRadius"));
		
		addcomp(COMP_CA::DOMAIN_COMP, domain);
	}
	if (luabool("HealComp")) {
//		CCLOG("creating heal comp");
		entity->components[COMP_CA::HEAL_COMP] = world->commonComps[COMP_CA::HEAL_COMP];
		//heal->healSpeed = luafloat("attackSpeed");
	}
	
	if (luabool("HordeStatusComp")) {
		CCLOG("creating hordestatus comp");
		auto hordeStatus = newcomp(HordeStatusComp, COMP_CA::HORDE_STATUS_COMP);
		hordeStatus->init();
		//set all the level to 0
		for (int i = ZOMBIE_CA::ZOMBIE_START + 1; i < ZOMBIE_CA::ZOMBIE_END; i++) {
			hordeStatus->zombieLvs[(ZOMBIE_CA)i] = 0;
		}
		
		//hordeStatus->zombieCounts[ZOMBIE_CA::STINKIE] = luaint("StinkieNum");
		//hordeStatus->zombieCounts[ZOMBIE_CA::CHUCKER] = luaint("ChuckerNum");
		//hordeStatus->zombieCounts[ZOMBIE_CA::HOLY_BONE] = luaint("HolyBondNum");
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::STINKIE_HP] = float(luaint("stinkieHP"));
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::STINKIE_ATTACK] = float(luaint("stinkieAttack"));
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::STINKIE_ATTACKSPEED] = luafloat("stinkieAttackSpeed");
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::STINKIE_RANGE] = float(luaint("stinkieRange"));
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::CHUCKER_HP] = luaint("chuckerHP");
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::CHUCKER_ATTACK] = luaint("chuckerAttack");
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::CHUCKER_ATTACKSPEED] = luafloat("chuckerAttackSpeed");
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::CHUCKER_RANGE] = luaint("chuckerRange");
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::HOLY_BONE_HP] = luaint("holyBoneHP");
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::HOLY_BONE_HEAL] = luaint("holyBoneHeal");
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::HOLY_BONE_ATTACK_SPEED] = luafloat("holyBoneAttackSpeed");
		//hordeStatus->zombieStat[ZOMBIE_STAT_CA::HOLY_BONE_RANGE] = luaint("holyBoneRange");


//		CCLOG("holy bone STAT IS %lf %lf, %lf, %lf", hordeStatus->zombieStat[ZOMBIE_STAT_CA::HOLY_BONE_HP], hordeStatus->zombieStat[ZOMBIE_STAT_CA::HOLY_BONE_HEAL], hordeStatus->zombieStat[ZOMBIE_STAT_CA::HOLY_BONE_ATTACK_SPEED], hordeStatus->zombieStat[ZOMBIE_STAT_CA::HOLY_BONE_RANGE]);
//		CCLOG("stinkie STAT IS %lf %lf, %lf, %lf", hordeStatus->zombieStat[ZOMBIE_STAT_CA::STINKIE_HP], hordeStatus->zombieStat[ZOMBIE_STAT_CA::STINKIE_ATTACK], hordeStatus->zombieStat[ZOMBIE_STAT_CA::STINKIE_ATTACKSPEED], hordeStatus->zombieStat[ZOMBIE_STAT_CA::STINKIE_RANGE]);
//		CCLOG("chucker STAT IS %lf %lf, %lf, %lf", hordeStatus->zombieStat[ZOMBIE_STAT_CA::CHUCKER_HP], hordeStatus->zombieStat[ZOMBIE_STAT_CA::CHUCKER_ATTACK], hordeStatus->zombieStat[ZOMBIE_STAT_CA::CHUCKER_ATTACKSPEED], hordeStatus->zombieStat[ZOMBIE_STAT_CA::CHUCKER_RANGE]);

		int total = 0;
		/*for (int i = ZOMBIE_CA::ZOMBIE_START; i < ZOMBIE_CA::ZOMBIE_END; i++) {
			total += hordeStatus->zombieCounts[(ZOMBIE_CA)i];
		}*/
		hordeStatus->total = total;
		addcomp(COMP_CA::HORDE_STATUS_COMP, hordeStatus);
		
	}
	if (luabool("KineticComp")) {
//		CCLOG("creating kinetic comp");
		auto kinetic = newcomp(KineticComp, COMP_CA::KINETIC_COMP);
		kinetic->init();
		//kinetic->maxSpeed = luafloat("KineticCompMaxSpeed");
		kinetic->maxSpeed = luafloat("kinetic_maxSpeed");
		float x, y;
		kinetic->pos.set(luafloat("kinetic_posX"), luafloat("kinetic_posY"));
		kinetic->vel.set(luafloat("kinetic_velX"), luafloat("kinetic_velY"));

		addcomp(COMP_CA::KINETIC_COMP, kinetic);
	}
	if (luabool("PointComp")) {
		//		CCLOG("creating point comp");

		auto pointComp = newcomp(PointComp, COMP_CA::POINT_COMP);
		pointComp->init();
		addcomp(COMP_CA::POINT_COMP, pointComp);
	}
	if (luabool("PowerUpComp")) {
		auto powerUpComp = newcomp(PowerUpComp, COMP_CA::POWER_UP_COMP);
		powerUpComp->init();
		addcomp(COMP_CA::POWER_UP_COMP, powerUpComp);
	}
	if (luabool("TrajectComp")) {
		auto trajectComp = newcomp(TrajectComp, COMP_CA::TRAJECT_COMP);
		trajectComp->init();
		addcomp(COMP_CA::TRAJECT_COMP, trajectComp);
	}

	if (luabool("WanderingComp")) {
//		CCLOG("creating wanderingComp");
		auto wanderingComp = newcomp(WanderingComp, COMP_CA::WANDERING_COMP);
		wanderingComp->init();
		addcomp(COMP_CA::WANDERING_COMP, wanderingComp);
	}
//	CCLOG("finish entity initialization");
}

Player* EntityFactory::playerFromLua(LuaTable& luaTable){
	World* world = World::instance();
	Player* player = world->getPlayerPool()->New();
	player->init();
	
	initEntity(player, luaTable);
	zombie_stat_helper::getZombieStatsFromLua(player);
	//add player to the world
	world->playerList.push_back(player);
	AttachmentFactory::createSpawnAtt(player);
	//player->tint(Color3B::MAGENTA);//test, tint all the player to magenta, TODO::remove it. read from lua.
	return player;
}

Zombie* EntityFactory::zombieFromLua(Player* player, LuaTable& luaTable) {
	CCLOG("creating zombie---------------------------");
	World* world = World::instance();
	Zombie* zombie = world->getZombiePool()->New();
	zombie->init();
	zombie->category = (ZOMBIE_CA)luaint("category");
	zombie->player = player;
	initEntity(zombie, luaTable);
	zombie_stat_helper::applyStat(player, zombie);
	auto hordeStatus = (HordeStatusComp*)player->components[COMP_CA::HORDE_STATUS_COMP];
	if (hordeStatus) {
		hordeStatus->total++;
		hordeStatus->zombieCounts[zombie->category]++;
	}
	world->zombieList.push_back(zombie);
	CCLOG("finish creating zombie---------------------");
	return zombie;
}

Zombie* EntityFactory::createZombie(Player* player,LuaFunction<LuaTable()>& luaFunc) {
	auto luaTable = luaFunc.Invoke();
	Zombie* zombie = zombieFromLua(player, luaTable);
	return zombie;
}

Zombie* EntityFactory::createStrayZombie(ZOMBIE_CA number) {
	World* world = World::instance();
	Zombie* zombie = world->getZombiePool()->New();
	zombie->init();
	auto lua = LuaDevice::instance();
	auto luaFunc = lua->global().Get<LuaFunction<LuaTable(bool)>>("createBasicStinkie");
	if (number == ZOMBIE_CA::HOLY_BONE) {
		luaFunc = lua->global().Get<LuaFunction<LuaTable(bool)>>("createBasicHolyBone");
	}
	else if (number == ZOMBIE_CA::CHUCKER) {
		luaFunc = lua->global().Get<LuaFunction<LuaTable(bool)>>("createBasicChucker");
	}
	auto luaTable = luaFunc.Invoke(false);
	initEntity(zombie, luaTable);
	zombie->category = (ZOMBIE_CA)luaint("category");
	zombie->player = NULL;
	world->zombieList.push_back(zombie);
	//play spawn effect
	AttachmentFactory::createSpawnAtt(zombie);
	return zombie;
}

Player* EntityFactory::createPlayer(bool isHuman,bool isBoss, float gameTime) {
	//load lua file, 
	auto lua = LuaDevice::instance();
	LuaTable global = lua->global();
	auto luaTable = global.Get<LuaFunction<LuaTable(bool, int)>>("newPlayer").Invoke(isHuman, PLAYER_CA::SWISS);
	Player* player = playerFromLua(luaTable);
	if (isHuman) {
		player->tint(Color3B(100, 255, 131));//light blue
	}
	else {
		player->tint(Color3B(253, 100, 255));
	}
	if (isBoss) {
		player->tint(Color3B(255, 100, 100));
	}

	
	return player;
}
Item* EntityFactory::createBullet(Entity* user, Vec2 destination) {
	World* world = World::instance();
	Item* entity = world->getItemPool()->New();
	entity->init();
	//TODO load data from lua file
	//for now. manually create the components. 
//1. kinetic comp
	auto kinComp = newcomp(KineticComp, COMP_CA::KINETIC_COMP);
	kinComp->init();
	auto userKinComp =(KineticComp*)user->components[KINETIC_COMP];
	if (userKinComp) {
		kinComp->pos.set(userKinComp->pos);//set the original position as the zombie's postion
		addcomp(COMP_CA::KINETIC_COMP, kinComp);
		CCLOG("item created kinetic comp");
	}
	
//2. domain comp
	auto domainComp = newcomp(DomainComp, COMP_CA::DOMAIN_COMP);
	domainComp->init();
	domainComp->radius = 100.0f;//splash damage within 100 px radius
	addcomp(COMP_CA::DOMAIN_COMP, domainComp);
//3. alliance
	entity->alliance = user->alliance;
	//TODO: change all the alliance calculation to entity instead of combat component
//4. TrajectComp: new comp, mark that this is a bullet. also record damage. Note that bullet doesn't have combat comp, so it wont be attacked. 
	auto trajectComp = newcomp(TrajectComp, COMP_CA::TRAJECT_COMP);
	trajectComp->init();
	trajectComp->target.set(destination);
	trajectComp->origin.set(kinComp->pos);
	addcomp(COMP_CA::TRAJECT_COMP, trajectComp);
//5. AnimComp
	auto animComp = newcomp(AnimComp, COMP_CA::ANIM_COMP);
	animComp->init();
	animComp->name = EFFECT_STR;
	//animComp->animState = A_FLY;
	animComp->newAnimState = A_SPIT_FLY;
	animComp->directional = 0;
	animComp->defaultAction = A_SPIT_FLY;
	addcomp(COMP_CA::ANIM_COMP, animComp);


	world->itemList.push_back(entity);
	return entity;
}

Item* EntityFactory::createPickUp(ITEM_CA type, Vec2 location) {
	World* world = World::instance();
	Item* entity = world->getItemPool()->New();
	entity->init();
	//TODO load data from lua file
	//for now. manually create the components. 
	//1. kinetic comp
	auto kinComp = newcomp(KineticComp, COMP_CA::KINETIC_COMP);
	kinComp->init();
	kinComp->pos.set(location);
	addcomp(COMP_CA::KINETIC_COMP, kinComp);
	CCLOG("item created kinetic comp");
	
	//2. domain comp, use to check if character touches it
	auto domainComp = newcomp(DomainComp, COMP_CA::DOMAIN_COMP);
	domainComp->init();
	domainComp->radius = 50.0f;//character within 50 can pick up
	addcomp(COMP_CA::DOMAIN_COMP, domainComp);
	//3. alliance, well neutral
	entity->alliance = 0;
	
	//4. AnimComp
	auto animComp = newcomp(AnimComp, COMP_CA::ANIM_COMP);
	animComp->init();
	animComp->name = EFFECT_STR;
	//animComp->animState = A_FLY;
	switch (type) {
	case ITEM_CA::ITEM_ATTACK:
		animComp->newAnimState = I_PICKUP_ATTACK;
		animComp->defaultAction = I_PICKUP_ATTACK;
		break;
	case ITEM_CA::ITEM_SPEED:
		animComp->newAnimState = I_PICKUP_SPEED;
		animComp->defaultAction = I_PICKUP_SPEED;
		break;
	case ITEM_CA::ITEM_INVIN:
		animComp->newAnimState = I_PICKUP_SHIELD;
		animComp->defaultAction = I_PICKUP_SHIELD;
		break;
	case ITEM_CA::ITEM_HEALTH:
		animComp->newAnimState = I_PICKUP_HEALTH;
		animComp->defaultAction = I_PICKUP_HEALTH;
		break;
	}
	animComp->directional = 0;
	addcomp(COMP_CA::ANIM_COMP, animComp);
	//5. powerUpComp, the most important one
	auto powerComp = newcomp(PowerUpComp, COMP_CA::POWER_UP_COMP);
	powerComp->init();
	powerComp->type = type;
	if (type == ITEM_CA::ITEM_HEALTH || type == ITEM_CA::ITEM_NONE) {
		powerComp->instant = true;
	}
	addcomp(COMP_CA::POWER_UP_COMP, powerComp);

	world->itemList.push_back(entity);
	return entity;
}