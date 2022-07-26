#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

class Map;

class CollisionMap
{
public:
	CollisionMap(int width, int height);

	void setColl(int x, int y, bool state);

	bool getColl(int x, int y);

	void setMapCollision(const Map& map);

public:
	int width, height;

private:
	std::vector<bool> Collision;
};
