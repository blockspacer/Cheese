#include "Entity.h"
#include "world/World.h"
#include "cocos2d.h"
#include "world/world_include.h"
USING_NS_CC;
#define DELETE_COMP(T, e) do{World::instance()->getCompPool<T>(e)->Delete((T*)this->components[e]); this->components[e]=NULL;}while(0)
Entity::Entity() {
	init();
}
void Entity::init() {
	marked = false;
	sprite = NULL;
	color = Color3B::WHITE; //initially the tint color is white, which will not affect the sprite color;
}
Entity::~Entity() {
	//TODO
	//free all resource
	cleanup();
	//set all pointers to NULL
	//remove all the components from the component list
	sprite = NULL;
	alliance = 0;
	World* world = World::instance();
	if (this == world->swiss) {
		world->swiss = NULL;
		world->destroyFlag = true;
	}
}

void Entity::cleanup() {
	//CCLOG("recycling entity");
	//remove all component
	World* world = World::instance();
	//only need to clean up the pooled comp
	//ActionFlagComp
	//world->getCompPool<ActionFlagComp>(COMP_CA::ACTION_FLAG_COMP)->Delete((ActionFlagComp*)this->components[COMP_CA::ACTION_FLAG_COMP]);
	DELETE_COMP(ActionFlagComp, COMP_CA::ACTION_FLAG_COMP);
	DELETE_COMP(AnimComp, COMP_CA::ANIM_COMP);
	DELETE_COMP(CombatComp, COMP_CA::COMBAT_COMP);
	DELETE_COMP(DomainComp, COMP_CA::DOMAIN_COMP);
	DELETE_COMP(HordeStatusComp, COMP_CA::HORDE_STATUS_COMP);
	DELETE_COMP(KineticComp, COMP_CA::KINETIC_COMP);
	DELETE_COMP(PointComp, COMP_CA::POINT_COMP);
	DELETE_COMP(TrajectComp, COMP_CA::TRAJECT_COMP);
	DELETE_COMP(WanderingComp, COMP_CA::WANDERING_COMP);
	//clean up the component array
	this->components.clear();
	this->removeAllAttachment();
}

void Entity::addAttachment(Attachment* attachment) {
	//iterate over attachment, see if any attachment is unfinished, 
	//if there is unfinished attachment, cannot add not queueable attachment
	if (!attachment) {
		CCLOG("ERROR::attachment is NULL");
		return;
	}
	if (!attachment->queueable) {
		auto attIt = attachments.begin();
		while (attIt != attachments.end()) {
			Attachment* att = (Attachment*)(*attIt);
			if (att && !att->finished)
				attIt++;
		}
		CCLOG("attachment not queueable, but the queue is busy");
		return;
	}
	else {
		attachments.push_back(attachment);
		attachment->setUserData(this);
	}
}
void Entity::removeAttachment(std::string name) {
	auto attIt = this->attachments.begin();
	CCLOG("removing %s in entity", name.c_str());
	while (attIt != this->attachments.end()) {
		Attachment* att = (Attachment*)(*attIt);
		CCLOG("att is %s", att->name.c_str());
		if (att && att->name == name) {
			CCLOG("Removing %s", name.c_str());
			att->finish();
		}
		attIt++;
	}
}

void Entity::removeAllAttachment() {
	auto attIt = this->attachments.begin();
	while (attIt != this->attachments.end()) {
		Attachment* att = (Attachment*)(*attIt);
		if (att) {
			att->finish();
		}
		attIt=this->attachments.erase(attIt);
	}
}
void Entity::tint(Color3B color) {
	this->color = color;
	if (this->sprite) {
		this->sprite->setColor(color);
	}
}
void Entity::tint() {
	if (this->sprite) {
		this->sprite->setColor(color);
	}
}
void Entity::untint() {
	this->color = Color3B::WHITE;
	if (this->sprite) {
		this->sprite->setColor(Color3B::WHITE);
	}
}
Color3B Entity::getColor() {
	if (this->sprite) {
		return this->sprite->getColor();
	}
	else {
		return Color3B::WHITE;
	}
}