#pragma once

enum class Tile
{
	Unused,

	/* Dungeon */
	Floor,
	Corridor,
	Wall,
	ClosedDoor,
	OpenDoor,
	UpStairs,
	DownStairs,

	/* Forest */
	Grass,
	Grass1,
	Grass2,
	Grass3,
	Grass4,
	Grass5,
	Grass6,
	Grass7,
	Grass8,
	Grass9,
	Grass10,
	Grass11,
	Grass12,
	Grass13,
	Tree,
	Tree1,
	Tree2,
	ClosedGate,
	OpenGate,
	Water,
	Water1,
	Water2,
	Water3,
	Water4,
	Bridge,
	Bridge1,
	Bridge2,
	Bridge3,

	/* Cave */
	Dirt,
	CaveWall,
	Lava,
	// Pit,

	/* Spaceship */
	Void,
	VoidWall,
};
