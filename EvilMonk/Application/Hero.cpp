#include "Hero.hpp"
#include "../Map/Map.hpp"
#include "../Utility/Utility.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <string>
#include <time.h>
#include <iostream>


Hero::Hero(CollisionMap& coll, const sf::Texture& tileset, const sf::Vector2i& tileSize, sf::Vector2i screenSize)
	: tileset(tileset)
	, tileSize(tileSize)
	, HeroPosition(-1, -1)
	, screenSize(screenSize)
	, coll(coll)
	, poisonFrames(0)
	, confuseFrames(0)
{
	moveVec.clear();



	switch (bool i = true)
	{
	case true:
		hp = 15; maxHp = 15; lvl = 1; AC = 12; damage = 8; type = "Fighter"; xp = 0;
		break;
	default:
		hp = 15; maxHp = 15; lvl = 1; AC = 12; damage = 8; type = "Fighter"; xp = 0;
		break;
	}

}

int Hero::isWall(const Map& map, int x, int y)
{
	switch (map.getTile(x, y))
	{
	case Tile::Tree:
		return true;
	case Tile::Tree1:
		return true;
	case Tile::Tree2:
		return true;
	case Tile::Water:
		return true;
	case Tile::Water1:
		return true;
	case Tile::Water2:
		return true;
	case Tile::Water3:
		return true;
	case Tile::Water4:
		return true;
	} 
	return false;
}

void Hero::Spawn(const Map& map, int SizeX, int SizeY, int actionSizeX, int actionSizeY)
{
	coll.setColl(HeroPosition.x, HeroPosition.y, false);
	do {
		HeroPosition.x = randomInt((actionSizeX / 2) + 1, SizeX - (actionSizeX / 2) - 1);
		HeroPosition.y = randomInt((actionSizeY / 2) + 1, SizeY - (actionSizeY / 2) - 1);
	} while (coll.getColl(HeroPosition.x, HeroPosition.y));
	coll.setColl(HeroPosition.x, HeroPosition.y, true);
	hp = 15; maxHp = 15; lvl = 1; AC = 12; damage = 8; type = "Fighter"; xp = 0;
}

sf::Vector2i Hero::getPosition()
{
	sf::Vector2i result;
	result.x = HeroPosition.x;
	result.y = HeroPosition.y;
	return result;
}

sf::Vector2i Hero::makePathStep(const Map& map, int dx, int dy)
{ 
	if (moveVec.size() > 0)
	{
		int xMov = moveVec[moveVec.size() - 1].x;
		int yMov = moveVec[moveVec.size() - 1].y;
		if (xMov > 0)
			left = false;
		if (xMov < 0)
			left = true;
		if (coll.getColl(HeroPosition.x + xMov, HeroPosition.y + yMov)) {
			moveVec.clear();
			war = true;
			attackCords = sf::Vector2i(HeroPosition.x + xMov, HeroPosition.y + yMov);
			return sf::Vector2i(dx, dy);
		}
		else
		{
			changeHp(1);
			coll.setColl(HeroPosition.x, HeroPosition.y, false);
			HeroPosition.x += xMov;
			HeroPosition.y += yMov;
			coll.setColl(HeroPosition.x, HeroPosition.y, true);
			moveVec.pop_back();
			return sf::Vector2i(dx + xMov, dy + yMov);
		}
	}
	return sf::Vector2i(dx, dy);
}

bool Hero::move(const Map& map, MoveHelper& moveHelper, int xMov, int yMov, int dx, int dy, bool native)
{
	moveVec.clear();
	if (native)
	{
		if (xMov > 0)
			left = false;
		if (xMov < 0)
			left = true;
		if (coll.getColl(HeroPosition.x + xMov, HeroPosition.y + yMov)) {
			war = true;
			attackCords = sf::Vector2i(HeroPosition.x + xMov, HeroPosition.y + yMov);
			return false;
		}
		else
		{
			changeHp(1);
			coll.setColl(HeroPosition.x, HeroPosition.y, false);
			HeroPosition.x += xMov;
			HeroPosition.y += yMov;
			coll.setColl(HeroPosition.x, HeroPosition.y, true);
			return true;
		}
	}
	else
	{
		int start_time = clock();
		moveVec = moveHelper.getMoveList(coll, HeroPosition.x - dx, HeroPosition.y - dy, HeroPosition.x - dx + xMov, HeroPosition.y - dy + yMov, dx, dy, screenSize);
		int end_time = clock();
		std::cout << end_time - start_time << '\n';
	}

	return false;
}

void Hero::load(int dx, int dy)
{
	vertices.clear();
	vertices.resize(4);

	vertices[0].position = sf::Vector2f((HeroPosition.x - dx + 0.f) * tileSize.x, (HeroPosition.y - dy + 0.f) * tileSize.y);
	vertices[1].position = sf::Vector2f((HeroPosition.x - dx + 1.f) * tileSize.x, (HeroPosition.y - dy + 0.f) * tileSize.y);
	vertices[2].position = sf::Vector2f((HeroPosition.x - dx + 1.f) * tileSize.x, (HeroPosition.y - dy + 1.f) * tileSize.y);
	vertices[3].position = sf::Vector2f((HeroPosition.x - dx + 0.f) * tileSize.x, (HeroPosition.y - dy + 1.f) * tileSize.y);
	
	if (!left)
	{
		vertices[0].texCoords = sf::Vector2f((0 + 0.f) * 32, (0 + 0.f) * 32);
		vertices[1].texCoords = sf::Vector2f((0 + 1.f) * 32, (0 + 0.f) * 32);
		vertices[2].texCoords = sf::Vector2f((0 + 1.f) * 32, (0 + 1.f) * 32);
		vertices[3].texCoords = sf::Vector2f((0 + 0.f) * 32, (0 + 1.f) * 32);
	}
	else
	{
		vertices[1].texCoords = sf::Vector2f((0 + 0.f) * 32, (0 + 0.f) * 32);
		vertices[0].texCoords = sf::Vector2f((0 + 1.f) * 32, (0 + 0.f) * 32);
		vertices[3].texCoords = sf::Vector2f((0 + 1.f) * 32, (0 + 1.f) * 32);
		vertices[2].texCoords = sf::Vector2f((0 + 0.f) * 32, (0 + 1.f) * 32);
	}

}

void Hero::addXp(int addXp)
{
	std::cout << "You gain " << addXp << " XP" << '\n';
	xp += addXp;
	std::cout << "XP: " << xp << '/' << needXp[lvl] << '\n';
	if (xp >= needXp[lvl])
	{
		xp = xp - needXp[lvl];
		levelUp();
	}
}

bool Hero::changeHp(int changeHp)
{
	hp = hp + changeHp;

	if (hp <= 0)
	{
		hp = 0;
		std::cout << "You take damage!!! Current hp: " << hp << "/" << maxHp << '\n';
		return false;
	}
	if (hp > maxHp)
	{
		hp = maxHp;
	}
	else
	{
		if (changeHp > 0)
			std::cout << "You restore hp.   Current hp: " << hp << "/" << maxHp << '\n';
		else
			std::cout << "You take damage!!! Current hp: " << hp << "/" << maxHp << '\n';
	}
	return true;
}

void Hero::levelUp()
{
	srand(time(NULL));
	maxHp += (rand() % 10) + 1;
	hp = maxHp;
	lvl++;
	std::cout << "Congratz, You level up and have " << lvl << " level" << '\n' << "HP: " << hp << "/" << maxHp << '\n';  
}
	
int Hero::getAC()
{
	return AC;
}

int Hero::getLvl()
{
	return lvl;
}

void Hero::attack(std::vector<Monster>& entity, bool& toupt)
{
	srand(clock());
	if (war)
	for (int i = 0; i < entity.size(); i++)
	{
		if (entity[i].getPosition().x == attackCords.x && entity[i].getPosition().y == attackCords.y)
		{
			toupt = true;
			if ((rand() % 20) + 1 + lvl > entity[i].getAC() + entity[i].getLvl())
			{
				int dealDamage = rand() % damage + 1 + lvl;
				std::cout << "You deal " << dealDamage << " damage. Nice!\n";
				if (!entity[i].changeHp(-dealDamage))
				{
					entity[i].die(coll, *this);
				}
			}
			else
			{
				std::cout << "You miss\n";
			}

		}
	}
	war = false;
}

int Hero::getXp()
{
	return xp;
}

int Hero::getNeedXp()
{
	return needXp[lvl];
}

int Hero::getHp()
{
	return hp;
}

int Hero::getMaxHp()
{
	return maxHp;
}

int Hero::getPoison()
{
	return poisonFrames;
}

void Hero::setPoison(int newValue)
{
	poisonFrames = newValue;
	if (poisonFrames < 0)
		poisonFrames = 0;
}

int Hero::getConfuse()
{
	return confuseFrames;
}

void Hero::setConfuse(int newValue)
{
	confuseFrames = newValue;
	if (confuseFrames < 0)
		confuseFrames = 0;
}

void Hero::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &tileset;
	target.draw(&vertices[0], vertices.size(), sf::Quads, states);
}


