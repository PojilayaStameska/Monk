#include "../Application/CollisionMap.hpp"
#include "../Map/Map.hpp"

CollisionMap::CollisionMap(int width, int height):
	width(width),
	height(height)
{
	Collision.resize(width * height);
}

void CollisionMap::setMapCollision(const Map& map)
{
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		{
			if (map.getTile(x, y) == Tile::Water || map.getTile(x, y) == Tile::Water1 || map.getTile(x, y) == Tile::Water2 || map.getTile(x, y) == Tile::Water3 ||
				map.getTile(x, y) == Tile::Water4 || map.getTile(x, y) == Tile::Tree || map.getTile(x, y) == Tile::Tree1 || map.getTile(x, y) == Tile::Tree2)
				Collision[y * width + x] = true;
			else
				Collision[y * width + x] = false;
		}
}

void CollisionMap::setColl(int x, int y, bool state)
{
	if (y >= 0 && x >= 0 && x < width && y < height)
		Collision[y * width + x] = state;
}

bool CollisionMap::getColl(int x, int y)
{
	return Collision[y * width + x];
}