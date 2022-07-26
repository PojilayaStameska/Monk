#include "Tilemap.hpp"
#include "../Map/Map.hpp"
#include "../Utility/Utility.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Tilemap::Tilemap(const sf::Texture& tileset, const sf::Vector2i& tileSize, sf::Vector2i screenSize)
	: tileset(tileset)
	, tileSize(tileSize)
	, screenSize(screenSize)
{
}

void Tilemap::load(Map& map, int dx, int dy)
{
	vertices.clear();
	vertices.resize((screenSize.x+1)* screenSize.y * 4);

	for (int y = dy; y < dy+screenSize.y; ++y)
		for (int x = dx; x < dx + screenSize.x; ++x)
		{	
			int tileNumber = getTileNumber(map, x, y);

			if (tileNumber < 0)
				continue;

			int tu = tileNumber % (tileset.getSize().x / 36);
			int tv = tileNumber / (tileset.getSize().x / 36);

			sf::Vertex* quad = &vertices[(x-dx + (y-dy) * screenSize.x) * 4];

			quad[0].position = sf::Vector2f((x-dx + 0.f) * tileSize.x, (y-dy + 0.f) * tileSize.y);
			quad[1].position = sf::Vector2f((x-dx + 1.f) * tileSize.x, (y-dy + 0.f) * tileSize.y);
			quad[2].position = sf::Vector2f((x-dx + 1.f) * tileSize.x, (y-dy + 1.f) * tileSize.y);
			quad[3].position = sf::Vector2f((x-dx + 0.f) * tileSize.x, (y-dy + 1.f) * tileSize.y);


			quad[0].texCoords = sf::Vector2f((tu + 0.f) * 36, (tv + 0.f) * 36);
			quad[1].texCoords = sf::Vector2f((tu + 1.f) * 36, (tv + 0.f) * 36);
			quad[2].texCoords = sf::Vector2f((tu + 1.f) * 36, (tv + 1.f) * 36);
			quad[3].texCoords = sf::Vector2f((tu + 0.f) * 36, (tv + 1.f) * 36);
		}
}


int Tilemap::getTileNumber(Map& map, int x, int y)
{
	auto isWall = [&] (int x, int y)
	{
		switch (map.getTile(x, y))
		{
		case Tile::Tree:
			return true;
		}

		return false;
	};


	int tileNumber = -1;

	auto addShadow = [&](int x, int y)
	{
		tileNumber = 0xaf;
	};

	switch (map.getTile(x, y))
	{

	case Tile::Grass:
		if (randomInt(3) > 0)
			tileNumber = 0x09;
		else if (randomInt(3) > 0)
			tileNumber = 0x09 + randomInt(1, 2);
		else
			tileNumber = 0x09 + randomInt(3, 5);

		if (randomInt(9) == 0)
		{
			tileNumber = 0x09;

			if (randomInt(9) > 0)
				tileNumber = 0x0f + randomInt(5);		
			else
				tileNumber = 0x10 + randomInt(5);
		}
		break;

	case Tile::Tree:

		if (randomInt(3) > 0)
			tileNumber = 0x0;

		else
			tileNumber = 0x1;
		break;

	case Tile::Water:
		if (y > 0 && map.getTile(x, y - 1) != Tile::Water && map.getTile(x, y - 1) != Tile::Water4 && map.getTile(x, y - 1) != Tile::Water1 && map.getTile(x, y - 1) != Tile::Water2 && map.getTile(x, y - 1) != Tile::Water3)
		{
			tileNumber = 0x4 + randomInt(2);
		}
		else
		{
			tileNumber = 0x2 + randomInt(2);
		}
		break;

	case Tile::Bridge:
		tileNumber = 0x06 + randomInt(3);
		break;

	case Tile::Tree1:

		tileNumber = 0x0;
		break;

	case Tile::Tree2:

		tileNumber = 0x1;
		break;

	case Tile::Water1:

		tileNumber = 0x2;
		break;

	case Tile::Water2:

		tileNumber = 0x3;
		break;

	case Tile::Water3:

		tileNumber = 0x4;
		break;

	case Tile::Water4:

		tileNumber = 0x5;
		break;

	case Tile::Bridge1:

		tileNumber = 0x06;
		break;

	case Tile::Bridge2:

		tileNumber = 0x07;
		break;

	case Tile::Bridge3:

		tileNumber = 0x08;
		break;

	case Tile::Grass1:

		tileNumber = 0x09;
		break;

	case Tile::Grass2:

		tileNumber = 0x0a;
		break;

	case Tile::Grass3:

		tileNumber = 0x0b;
		break;

	case Tile::Grass4:

		tileNumber = 0x0c;
		break;

	case Tile::Grass5:

		tileNumber = 0x0d;
		break;

	case Tile::Grass6:

		tileNumber = 0x0e;
		break;

	case Tile::Grass7:

		tileNumber = 0x0f;
		break;

	case Tile::Grass8:

		tileNumber = 0x10;
		break;

	case Tile::Grass9:

		tileNumber = 0x11;
		break;

	case Tile::Grass10:

		tileNumber = 0x12;
		break;

	case Tile::Grass11:

		tileNumber = 0x13;
		break;

	case Tile::Grass12:

		tileNumber = 0x14;
		break;

	case Tile::Grass13:

		tileNumber = 0x15;
		break;

	}

	if (tileNumber == 0x06)
		map.setTile(x, y, Tile::Bridge1);
	if (tileNumber == 0x07)
		map.setTile(x, y, Tile::Bridge2);
	if (tileNumber == 0x08)
		map.setTile(x, y, Tile::Bridge3);

	if (tileNumber == 0x09)
		map.setTile(x, y, Tile::Grass1);
	if (tileNumber == 0x0a)
		map.setTile(x, y, Tile::Grass2);
	if (tileNumber == 0x0b)
		map.setTile(x, y, Tile::Grass3);
	if (tileNumber == 0x0c)
		map.setTile(x, y, Tile::Grass4);
	if (tileNumber == 0x0d)
		map.setTile(x, y, Tile::Grass5);
	if (tileNumber == 0x0e)
		map.setTile(x, y, Tile::Grass6);
	if (tileNumber == 0x0f)
		map.setTile(x, y, Tile::Grass7);
	if (tileNumber == 0x10)
		map.setTile(x, y, Tile::Grass8);
	if (tileNumber == 0x11)
		map.setTile(x, y, Tile::Grass9);
	if (tileNumber == 0x12)
		map.setTile(x, y, Tile::Grass10);
	if (tileNumber == 0x13)
		map.setTile(x, y, Tile::Grass11);
	if (tileNumber == 0x14)
		map.setTile(x, y, Tile::Grass12);
	if (tileNumber == 0x15)
		map.setTile(x, y, Tile::Grass13);

	if (tileNumber == 0x0)
		map.setTile(x, y, Tile::Tree1);
	if (tileNumber == 0x1)
		map.setTile(x, y, Tile::Tree2);

	if (tileNumber == 0x2)
		map.setTile(x, y, Tile::Water1);
	if (tileNumber == 0x3)
		map.setTile(x, y, Tile::Water2);
	if (tileNumber == 0x4)
		map.setTile(x, y, Tile::Water3);
	if (tileNumber == 0x5)
		map.setTile(x, y, Tile::Water4);

	return tileNumber;
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &tileset;
	target.draw(&vertices[0], vertices.size(), sf::Quads, states);
}
 