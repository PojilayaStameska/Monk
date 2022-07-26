#include "Application.hpp"
#include "WindowsHelper.hpp"

// Map Generators
#include "../Generator/Forest.hpp"
#include <SFML/Window/Event.hpp>
#include <iostream>


namespace
{ 
	const sf::Vector2i tileSize(46, 46);
	const sf::Vector2i mapSize(200, 120);
}

Application::Application()
	: window(sf::VideoMode(1280,720), "Game???", sf::Style::Close)
	, map(mapSize.x, mapSize.y)
	,collisionMap(mapSize.x, mapSize.y)
{
	centerWindow(window.getSystemHandle());
	window.setFramerateLimit(60);

	if (!texture.loadFromFile("newset.png"))
		printf("Error");
	if (!textureHero.loadFromFile("Sprite2.png"))
		printf("Error");
	if (!textureEnemy.loadFromFile("enemy.png"))
		printf("Error");
	font.loadFromFile("OpenSans-Regular.ttf");

	for (int i = 0; i < (mapSize.x * mapSize.y) / 160; i++)
	{
		std::unique_ptr<Monster> buff = std::make_unique<Monster>(textureEnemy);
		Entity.push_back(*buff);
	}

	moveHelper = std::make_unique<MoveHelper>(sf::Vector2i(window.getSize().x / tileSize.x, window.getSize().y / tileSize.y));
	tilemap = std::make_unique<Tilemap>(texture, tileSize, sf::Vector2i(window.getSize().x / tileSize.x, window.getSize().y / tileSize.y));
	hero = std::make_unique<Hero>(collisionMap, textureHero, tileSize, sf::Vector2i(window.getSize().x / tileSize.x, window.getSize().y / tileSize.y));
	lightmap = std::make_unique<Lightmap>(texture, sf::Vector2i(mapSize.x, mapSize.y), sf::Vector2i (tileSize.x, tileSize.y));
	hud = std::make_unique<HUD>(sf::Vector2f(window.getSize().x, window.getSize().y), font);
	registerGenerators();
	generateMap();

}

void Application::run()
{
	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		processInput();
		update(dt);
		render();
	}
}

void Application::processInput()
{
	int timenew = clock();

	sf::Event event;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();

		else if (event.type == sf::Event::KeyPressed)
		{
			if (toUpdate == false)
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				window.close();
				break;

			case sf::Keyboard::Space:
				rng.reset();
				generateMap();
				break;
				
			case sf::Keyboard::Left:
				selectPreviousGenerator();
				break;

			case sf::Keyboard::Right:
				selectNextGenerator();
				break;

			case sf::Keyboard::W:
				if (hero->move(map, *moveHelper, 0, -1, dx, dy, true) == true)
				{
					dy -= 1;
					if (dy < 0)
						dy = 0;
					toUpdate = true;
				}
				break;

			case sf::Keyboard::S:
				if (hero->move(map, *moveHelper, 0, 1, dx, dy, true) == true)
				{
					dy += 1;
					if (dy + window.getSize().y / tileSize.y > mapSize.y)
						dy = mapSize.y - window.getSize().y / tileSize.y;
					toUpdate = true;
					tilemap->load(map, dx, dy);
					hero->load(dx, dy);
					lightmap->makeSomeLight(map, dx, dy, hero->getPosition(), sf::Vector2f(window.getSize().x / tileSize.x, window.getSize().y / tileSize.y));
				}
				break;

			case sf::Keyboard::A:
				if (hero->move(map, *moveHelper, -1, 0, dx, dy, true) == true)
				{
					dx = dx - 1;
					if (dx < 0)
						dx = 0;
					toUpdate = true;
				}
				break;

			case sf::Keyboard::D:
				if (hero->move(map, *moveHelper, 1, 0, dx, dy, true) == true)
				{
					dx += 1;
					if (dx + window.getSize().x / tileSize.x > mapSize.x)
						dx = mapSize.x - window.getSize().x / tileSize.x;
					toUpdate = true;
				}
				break;
			}
			hero->attack(Entity, toUpdate);
			tilemap->load(map, dx, dy);
			hero->load(dx, dy);
			lightmap->makeSomeLight(map, dx, dy, hero->getPosition(), sf::Vector2f(window.getSize().x / tileSize.x, window.getSize().y / tileSize.y));
		}

		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i position = sf::Mouse::getPosition(window);
				sf::Vector2i moveDelta(position.x/tileSize.x - hero->getPosition().x + dx, position.y/tileSize.y - hero->getPosition().y + dy);

				if (lightmap->getShadow(hero->getPosition().x + moveDelta.x, hero->getPosition().y + moveDelta.y) > 0)
					hero->move(map, *moveHelper, moveDelta.x, moveDelta.y, dx, dy, false);

			}
			
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			int x = event.mouseMove.x / tileSize.x;
			int y = event.mouseMove.y / tileSize.y;
   			enemyType = "";
			for (int i = 0; i < Entity.size(); i++)
			{
				if (Entity[i].getPosition() == sf::Vector2i(x + dx, y + dy) && Entity[i].lifeState())
				{
					enemyType = Entity[i].getType();
					eLvl = Entity[i].getLvl();
					eHp = Entity[i].getHp();
					eMHp = Entity[i].getmaxHp();
				}
			}
		}
	}

	if (timenew - time > 150)
	{
		time = timenew;
		sf::Vector2i delta = hero->makePathStep(map, dx, dy);
		if (delta != sf::Vector2i(dx, dy))
		{
			dx = delta.x;
			dy = delta.y;
			if (dy < 0)
				dy = 0;
			if (dx + window.getSize().x / tileSize.x > mapSize.x)
				dx = mapSize.x - window.getSize().x / tileSize.x;
			if (dx < 0)
				dx = 0;
			if (dy + window.getSize().y / tileSize.y > mapSize.y)
				dy = mapSize.y - window.getSize().y / tileSize.y;
			toUpdate = true;
		}
		tilemap->load(map, dx, dy);
		hero->load(dx, dy);
		lightmap->makeSomeLight(map, dx, dy, hero->getPosition(), sf::Vector2f(window.getSize().x / tileSize.x, window.getSize().y / tileSize.y));
		hero->attack(Entity, toUpdate);
	}
}

void Application::update(sf::Time dt)
{

}

void Application::render()
{
	if (lose == false)
	{
		if (toUpdate)
		{

			std::cout << "NEW FRAME: \n";
			hero->setPoison(hero->getPoison() - 1);
			hero->setConfuse(hero->getConfuse() - 1);
			for (int i = 0; i < Entity.size(); i++)
			{
				sf::Vector2i pos = Entity[i].getPosition();
				Entity[i].move(collisionMap, *moveHelper, hero->getPosition(), sf::Vector2i(window.getSize().x / tileSize.x, window.getSize().y / tileSize.y),
					lightmap->getShadow(Entity[i].getPosition().x, Entity[i].getPosition().y), dx, dy);
				if (Entity[i].attack())
				{
					if (rng.getInt(20) + Entity[i].getLvl() > hero->getAC() + hero->getLvl())
					{
						if (!hero->changeHp(-(rng.getInt(Entity[i].getDamage()) + Entity[i].getLvl())))
							lose = true;
					}
					else
						std::cout << "You evade attack\n";
				}
				if (pos.x != Entity[i].getPosition().x || pos.y != Entity[i].getPosition().y)
				{
					window.clear();
					window.draw(*tilemap);
					window.draw(*hero);
					for (int i = 0; i < Entity.size(); i++)
					{
						Entity[i].load(dx, dy, tileSize, lightmap->getShadow(Entity[i].getPosition().x, Entity[i].getPosition().y));
						window.draw(Entity[i]);
					}
					window.draw(*lightmap);
					hud->load(hero->getHp(), hero->getMaxHp(), hero->getLvl(), hero->getXp(), hero->getNeedXp(), hero->getPoison(), hero->getConfuse(), enemyType, eLvl, eHp, eMHp);
					window.draw(*hud);
					window.display();
				}
				if (rng.getInt(5) == 0)
				{
					sf::Vector2i pos = Entity[i].getPosition();
					Entity[i].move(collisionMap, *moveHelper, hero->getPosition(), sf::Vector2i(window.getSize().x / tileSize.x, window.getSize().y / tileSize.y),
						lightmap->getShadow(Entity[i].getPosition().x, Entity[i].getPosition().y), dx, dy);
					if (Entity[i].attack())
					{
						if (rng.getInt(20) + Entity[i].getLvl() > hero->getAC())
						{
							if (!hero->changeHp(-(rng.getInt(Entity[i].getDamage()) + Entity[i].getLvl())))
								lose = true;
						}
						else
							std::cout << "You evade attack\n";
					}

					if (pos.x != Entity[i].getPosition().x || pos.y != Entity[i].getPosition().y)
					{
						window.clear();
						window.draw(*tilemap);
						window.draw(*hero);
						for (int i = 0; i < Entity.size(); i++)
						{
							Entity[i].load(dx, dy, tileSize, lightmap->getShadow(Entity[i].getPosition().x, Entity[i].getPosition().y));
							window.draw(Entity[i]);
						}
						window.draw(*lightmap);
						hud->load(hero->getHp(), hero->getMaxHp(), hero->getLvl(), hero->getXp(), hero->getNeedXp(),hero->getPoison(), hero->getConfuse(), enemyType, eLvl, eHp, eMHp);
						window.draw(*hud);
						window.display();
					}
				}
			}
			toUpdate = false;

		}
		window.clear();
		window.draw(*tilemap);
		window.draw(*hero);
		for (int i = 0; i < Entity.size(); i++)
		{
			Entity[i].load(dx, dy, tileSize, lightmap->getShadow(Entity[i].getPosition().x, Entity[i].getPosition().y));
			window.draw(Entity[i]);
		}
		window.draw(*lightmap);
		hud->load(hero->getHp(), hero->getMaxHp(), hero->getLvl(), hero->getXp(), hero->getNeedXp(),hero->getPoison(), hero->getConfuse(), enemyType, eLvl, eHp, eMHp);
		window.draw(*hud);
		window.display();
	}
	else
	{
		layer = 0;
		generateMap();
		lose = false;
	}

}

void Application::selectNextGenerator()
{
	currentGenerator = (currentGenerator + 1) % generators.size();
	rng.reset();
	generateMap();
}

void Application::selectPreviousGenerator()
{
	currentGenerator = (currentGenerator + generators.size() - 1) % generators.size();
	rng.reset();
	generateMap();
}

void Application::generateMap()
{
	srand(rng.getInt(1000000));
	layer++;
	sf::Clock clock;
	generators[currentGenerator]->generate(map, rng);
	sf::Time elapsedTime = clock.getElapsedTime();

	text.setString(generators[currentGenerator]->getName()
#ifdef _DEBUG
		+ L" (" + std::to_wstring(elapsedTime.asMilliseconds()) + L"ms)"
#else
		+ L" (Seed: " + std::to_wstring(rng.getSeed()) + L")"
#endif
	);
	lightmap->clear();



	collisionMap.setMapCollision(map);
	hero->Spawn(map, 200, 120, window.getSize().x / tileSize.x, window.getSize().y / tileSize.y);

	for (int i = 0; i < Entity.size(); i++)
	{
		Entity[i].spawn(collisionMap, layer, rand());
	}

	dx = hero->getPosition().x - (window.getSize().x / tileSize.x) / 2;
	dy = hero->getPosition().y - (window.getSize().y / tileSize.y) / 2;
	hero->load(dx, dy);
	tilemap->load(map, dx, dy);
	lightmap->makeSomeLight(map, dx, dy, hero->getPosition(), sf::Vector2f(window.getSize().x/tileSize.x, window.getSize().y/tileSize.y));
}

void Application::registerGenerators()
{
	generators.emplace_back(std::make_unique<OldForest>());
	generators.emplace_back(std::make_unique<MazyForest>());
	generators.emplace_back(std::make_unique<ForestAndLakes>());
	generators.emplace_back(std::make_unique<ForestAndRiver>());

}
