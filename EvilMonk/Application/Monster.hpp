#pragma once
#include "../Utility/MoveHelper.hpp"
#include "../Application/CollisionMap.hpp"
#include "Hero.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <string>
#include <memory>
#include <vector>

class map;
class Hero;

class Monster : public sf::Drawable, public sf::Transformable
{
public:
	Monster(sf::Texture& tileset);
	~Monster();

	int getAC();
	int getDamage();
	int getLvl();
	int getHp();
	int getmaxHp();
	bool lifeState();

	bool attack();
	bool changeHp(int changeHp);

	void move(CollisionMap& coll, MoveHelper& moveHelper, sf::Vector2i HeroPos, sf::Vector2i actionSize, int light, int dx, int dy);
	void spawn(CollisionMap& coll, int layer, int random);
	void load(int dx, int dy, sf::Vector2i tileSize, int light);
	void die(CollisionMap& coll, Hero& hero);

	std::string getType();

	sf::Vector2i getPosition();

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	bool war = false;
	const sf::Texture& tileset;
	std::vector<sf::Vector2i> moveVec;
	sf::Vector2i position;
	int AC = 0, hp = 0, damage = 0, lvl = 0, maxHp = 0;
	bool alive, left = true;
	std::vector<sf::Vertex> vertices;
	std::string type;
};

