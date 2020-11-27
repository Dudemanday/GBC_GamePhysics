#pragma once
#ifndef __GAME_OBJECT_TYPE__
#define __GAME_OBJECT_TYPE__
enum GameObjectType {
	NONE = -1,
	SHIP,
	BRICK,
	TARGET,
	PLAYER,
	PLANE,
	BULLET,
	START_BUTTON,
	RESTART_BUTTON,
	BACK_BUTTON,
	NEXT_BUTTON,
	RAMP,
	NUM_OF_TYPES
};
#endif /* defined (__GAME_OBJECT_TYPE__) */