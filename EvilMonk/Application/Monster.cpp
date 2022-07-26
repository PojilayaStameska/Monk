#include "Monster.hpp"
#include "..\Map\Map.hpp"
#include "..\Utility\Rng.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <string>
#include <time.h>
#include <iostream>


Monster::Monster(sf::Texture& tileset):
	tileset(tileset)
{
	alive = false;
	type = "NONE";
	moveVec.clear();
	position = sf::Vector2i(-1, -1);
}

Monster::~Monster()
{

}

void Monster::spawn(CollisionMap& coll, int layer, int random)
{
	std::cout << "spawn\n";
	srand(random);
	int typeId = 0;
	if (layer > 3){
		typeId = rand() % 10;
	} 
	else if (layer > 2){
		typeId = rand() % 9;
	}
	else {
		typeId = rand() % 8;
	}

	lvl = (rand() % (layer)) + layer;
	maxHp = 1;
	alive = true;

	coll.setColl(position.x, position.y, false);

	do {
		position.x = rand() % coll.width;
		position.y = rand() % coll.height;
	} while (coll.getColl(position.x, position.y));

	coll.setColl(position.x, position.y, true);

	switch (typeId)
	{
	case 0:
		type = "Kodold";
		for (int i = 0; i < lvl; i++)  //1d5 per level
			maxHp += rand() % 5;
		hp = maxHp;
		AC = 9;
		damage = 4; // will be calculated 1d"damage" + level
		break;
	case 1:
		type = "Spider";
		for (int i = 0; i < lvl; i++)  //1d5 per level
			maxHp += rand() % 5;
		hp = maxHp;
		AC = 9;
		damage = 6; 
		break;
	case 2:
		type = "Spider";
		for (int i = 0; i < lvl; i++)  //1d6 per level
			maxHp += rand() % 6;
		hp = maxHp;
		AC = 10;
		damage = 5;
		break;
	case 3:
		type = "Spider";
		for (int i = 0; i < lvl; i++)  //1d6 per level
			maxHp += rand() % 6;
		hp = maxHp;
		AC = 10;
		damage = 6;
		break;
	case 4:
		type = "Spider";
		for (int i = 0; i < lvl; i++)  //1d6 per level
			maxHp += rand() % 6;
		hp = maxHp;
		AC = 11;
		damage = 5; 
		break;
	case 5:
		type = "Wraith";
		for (int i = 0; i < lvl; i++)  //1d6 per level
			maxHp += rand() % 6;
		hp = maxHp;
		AC = 11;
		damage = 6;
		break;
	case 6:
		type = "Minotaur";
		for (int i = 0; i < lvl; i++)  //1d7 per level
			maxHp += rand() % 7;
		hp = maxHp;
		AC = 11;
		damage = 7; 
		break;
	case 7:
		type = "Troll";
		for (int i = 0; i < lvl; i++)  //1d8 per level
			maxHp += rand() % 8;
		hp = maxHp;
		AC = 8;
		damage = 8; 
		break;
	case 8:
		type = "Dragon";
		for (int i = 0; i < lvl; i++)  //1d10 per level
			maxHp += rand() % 10;
		hp = maxHp;
		AC = 12;
		damage = 10; 
		break;
	case 9:
		type = "Hydra";
		for (int i = 0; i < lvl; i++)  //1d12 per level
			maxHp += rand() % 12;
		hp = maxHp;
		AC = 13;
		damage = 12; 
		break;
	}
	maxHp *= 2;
	hp = maxHp;
}

bool Monster::attack()
{
	if (war) {
		war = false;
		return true;
	}
	return false;
}

void Monster::move(CollisionMap& coll, MoveHelper& moveHelper, sf::Vector2i HeroPos, sf::Vector2i actionSize, int light, int dx, int dy)
{
	if (alive)
	{
		if (light == 2)
		{
			int start_time = clock();
			std::cout << "moveClac\n";
			//if (abs(HeroPos.x - position.x) > abs(HeroPos.y - position.y))
			//	switch (HeroPos.x > position.x)
			//	{
			//	case(true):
			//	default:
			//		break;
			//	}
			//  MAKE HERE FASTER MOVE (WITHOUT PATHFINFING) (LIGHTMAP GIVES U INFO - NO OBSTACLES BETWEEEN HERO AND MONSTER)


			moveVec = moveHelper.getMoveList(coll, position.x - HeroPos.x + 8, position.y - HeroPos.y + 8, 8, 8, HeroPos.x - 8, HeroPos.y - 8, sf::Vector2i(16,16));
			int end_time = clock();
			std::cout << end_time - start_time << '\n';
		}


		if (moveVec.size() > 0)
		{
			int xMov = moveVec[moveVec.size() - 1].x;
			int yMov = moveVec[moveVec.size() - 1].y;
			if (xMov > 0)
				left = false;
			if (xMov < 0)
				left = true;
			if (position.x + xMov >= 199 || position.y + yMov >= 119)
			{
				alive = false;
			}
			else
			{
				if (coll.getColl(position.x + xMov, position.y + yMov)) {
					if (position.x + xMov == HeroPos.x && position.y + yMov == HeroPos.y)
					{
						war = true;
						std::cout << "war " << position.x << " " << position.y << " " << '\n';
					}
					moveVec.clear();
				}
				else
				{
					coll.setColl(position.x, position.y, false);
					position.x += xMov;
					position.y += yMov;
					coll.setColl(position.x, position.y, true);
					moveVec.pop_back();
				}
			}
		} 
		else
		{
			//make random move;
		}
	}
}

void Monster::die(CollisionMap& coll, Hero& hero)
{
	srand(time(NULL));
	alive = false;
	std::cout << type << " lvl " << lvl << " die  ";
	int xpGain = 0;
	for (int i = 0; i < lvl; i++)
		xpGain += (rand() % maxHp) + 1;
	coll.setColl(position.x, position.y, false);
	hero.addXp(xpGain * lvl / hero.getLvl());
}

bool Monster::changeHp(int changeHp)
{
	if (alive)
	{
		hp = hp + changeHp;
		if (hp <= 0)
		{
			hp = 1;
			return false;
		}
		return true;
	}
	return true;
}

int Monster::getAC()
{
	return AC;
}

int Monster::getDamage()
{
	return damage;
}

int Monster::getLvl()
{
	return lvl;
}

int Monster::getHp()
{
	return hp;
}

bool Monster::lifeState()
{
	return alive;
}

int Monster::getmaxHp()
{
	return maxHp;
}

sf::Vector2i Monster::getPosition()
{
	return position;
}

std::string Monster::getType()
{
	return type;
}


void Monster::load(int dx, int dy, sf::Vector2i tileSize, int light)
{
	vertices.clear();

	if (alive && light == 2)
	{
		vertices.resize(4);
		vertices[0].position = sf::Vector2f((position.x - dx + 0.f) * tileSize.x, (position.y - dy + 0.f) * tileSize.y);
		vertices[1].position = sf::Vector2f((position.x - dx + 1.f) * tileSize.x, (position.y - dy + 0.f) * tileSize.y);
		vertices[2].position = sf::Vector2f((position.x - dx + 1.f) * tileSize.x, (position.y - dy + 1.f) * tileSize.y);
		vertices[3].position = sf::Vector2f((position.x - dx + 0.f) * tileSize.x, (position.y - dy + 1.f) * tileSize.y);


		if (left)
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

		//vertices[0].color = sf::Color(255, 0, 0);
		//vertices[1].color = sf::Color(255, 0, 0);
		//vertices[2].color = sf::Color(255, 0, 0);
		//vertices[3].color = sf::Color(255, 0, 0);
	}

}


void Monster::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (vertices.size() > 0)
	{
		states.transform *= getTransform();
		states.texture = &tileset;
		target.draw(&vertices[0], vertices.size(), sf::Quads, states);
	}

}
