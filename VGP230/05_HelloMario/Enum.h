#pragma once

enum EntityType : int
{
	ET_NONE			= 0,  // 0000
	ET_PLAYER		 = 1 << 0, // 0001
	ET_TITLE		= 1 << 1, // 0010
	ET_ENEMY		= 1 << 2, // 0100
	ET_PICKUP		= 1 << 3, // 1000
	ET_PLAYER_PROJECTILE	= 1 << 4, // 0001 0000 
	ET_ENEMY_PROJECTILE		= 1 << 5  // 0010 0000
};

enum TileType : int
{
	TT_NONE	= 0, // 0 0000
	TT_GROUND, // 1 0001
	TT_WALL, // 2 0010
	TT_WATER, // 3 0011
	TT_DOOR, // 4 0100
};

enum MovementState : int
{
	MS_NONE = 0,
	MS_IDLE,
	MS_WALK,
	MS_JUMP,
	MS_FALL,
};