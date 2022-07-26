#pragma once

#include "Tilemap.hpp"
#include "Hero.hpp"
#include "Lightmap.hpp"
#include "HUD.hpp"
#include "../Map/Map.hpp"
#include "../Generator/Generator.hpp"
#include "../Utility/Rng.hpp"
#include "../Utility/MoveHelper.hpp"
#include "../Application/Monster.hpp"
#include "CollisionMap.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>

#include <memory> // unique_ptr

class Application
{
public:
	Application();

	void run();

private:
	void processInput();
	void update(sf::Time dt);
	void render();

	void selectNextGenerator();
	void selectPreviousGenerator();
	void generateMap();

	void registerGenerators();

private:
	sf::RenderWindow window;
	sf::Texture texture;
	sf::Texture textureHero;
	sf::Texture textureEnemy;
	sf::Font font;
	sf::Text text;
	sf::Text textMousePos;

	Map map;
	Rng rng;
	CollisionMap collisionMap;

	bool toUpdate = true;
	bool lose = false;
	int dy, dx;
	int time = 0;
	int layer = 0;

	std::string enemyType = "";
	int eLvl, eHp, eMHp;
	std::unique_ptr<MoveHelper> moveHelper;
	std::unique_ptr<Tilemap> tilemap;
	std::unique_ptr<Hero> hero;
	std::unique_ptr<Lightmap> lightmap;
	std::unique_ptr<HUD> hud;
	std::vector<Monster> Entity;
	std::vector<std::unique_ptr<Generator>> generators;
	std::size_t currentGenerator = 0;
};
