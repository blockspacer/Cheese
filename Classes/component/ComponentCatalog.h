#pragma once

enum COMP_CA {
	//basic component
	//to create, use "new", to remove, use "delete"
	KEYBOARD_COMP,
	CHASING_COMP,
	SEPERATION_COMP,
	FOLLOWING_COMP,
	MELEE_ATTACK_COMP,
	RECRUIT_COMP,
	RANGE_ATTACK_COMP,
	HEAL_COMP,
	//pooled component
	//to create, use "objectPool.New()" and then initialize the component, to remove use "pool.Delete(test)", this will call the destructor, but not deleting the object
	KINETIC_COMP,
	ANIM_COMP,
	DOMAIN_COMP,
	HORDE_STATUS_COMP,
	COMBAT_COMP,
	ACTION_FLAG_COMP,
	WANDERING_COMP,
	TRAJECT_COMP,
	POINT_COMP,
	POWER_UP_COMP


};