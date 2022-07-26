#pragma once

#include "../Utility/MoveHelper.hpp"
#include "../Application/CollisionMap.hpp"
#include "Monster.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <string>
#include <memory>
#include <vector>

class Monster;
class Map;
class CollisionMap;

class Hero : public sf::Drawable, public sf::Transformable
{
public:
	Hero(CollisionMap& coll, const sf::Texture& tileset, const sf::Vector2i& tileSize, sf::Vector2i screenSize);

	void Spawn(const Map& map, int SizeX, int SizeY, int actionSizeX, int actionSizeY);
	void load(int dx, int dy);
	bool move(const Map& map, MoveHelper& moveHelper, int xMov, int yMov, int dx, int dy, bool native);

	sf::Vector2i makePathStep(const Map& map, int dx, int dy);

	sf::Vector2i getPosition();

	//int wait(const Map& map);

	bool changeHp(int changeHp);
	void addXp(int addXp);
	void levelUp();

	/*std::string getType();*/

	int getHp();
	int getMaxHp();
	int getXp();
	int getNeedXp();

	int getAC();
	int getLvl();

	int getPoison();
	void setPoison(int newValue);

	int getConfuse();
	void setConfuse(int newValue);

	void attack(std::vector<Monster>& entity, bool& toupt);

private:

	int isWall(const Map& map, int x, int y);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	int maxHp, hp, lvl, AC, damage, xp, poisonFrames, confuseFrames;
	std::string type;
	int needXp[20]{ 0,50,150,250,500,750,1000,1500,2000,2500,3000,4000,5000,7500,10000,15000,20000,25000,30000,40000 };
	bool war = false;
	CollisionMap& coll;
	bool left = true;
	std::vector<sf::Vector2i> moveVec;
	const sf::Texture& tileset;
	const sf::Vector2i tileSize;
	sf::Vector2i screenSize, HeroPosition, attackCords;
	std::vector<sf::Vertex> vertices;
	
};
