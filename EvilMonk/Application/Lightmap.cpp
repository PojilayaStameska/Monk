#include "Lightmap.hpp"
#include "../Map/Map.hpp"
#include "../Utility/Utility.hpp"
#include "SPCVT.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
#include <ctime>

Lightmap::Lightmap(const sf::Texture& tileset, const sf::Vector2i actionSize, sf::Vector2i tileSize)
    :world(actionSize.x, actionSize.y),
    tileSize(tileSize),
    tileset(tileset)
{
	shadowValue.resize(world.y * world.x);
	for (int i = 0; i < world.y * world.x; i++)
	{
		shadowValue[i] = 0;
	}
}

void Lightmap::clear()
{
	shadowValue.clear();
    shadowValue.resize(world.y * world.x);
    for (int i = 0; i < world.y * world.x; i++)
    {
        shadowValue[i] = 0;
    }
}

void Lightmap::setShadow(int x, int y, int value)
{
    if (value == 1 && shadowValue[y * world.x + x] == 2)
        shadowValue[y * world.x + x] = 1;
    if (value == 2)
        shadowValue[y * world.x + x] = 2;
}

int Lightmap::getShadow(int x, int y)
{
    if (x < world.x && y < world.y)
        return shadowValue[y * world.x + x];
    else
        return -1;
}

void Lightmap::makeSomeLight(const Map& map, int dx, int dy, sf::Vector2i sourcePosInTiles, sf::Vector2f ScreenSize)
{
    vertices.clear();
    vertices.resize(ScreenSize.x * ScreenSize.y * 4*2);

    for (int x = dx; x < dx + ScreenSize.x; x++)
        for (int y = dy; y < dy + ScreenSize.y; y++)
        {
            this->setShadow(x, y, 1);
        };
    auto blocksVisibility = [&](int x, int y) {
        if (x < dx || x >= dx + ScreenSize.x || y < dy || y >= dy + ScreenSize.y) return true;
        return map.getTile(x,y) == Tile::Tree || map.getTile(x, y) == Tile::Tree1 || map.getTile(x, y) == Tile::Tree2;
    };

    auto setVisible = [&](int x, int y) {
    };

    SPCVT fov(6, false);
    fov.FOV(sourcePosInTiles.x - dx, sourcePosInTiles.y - dy, blocksVisibility, setVisible);

    int cx, cy;
    auto checkblock = [&](int x, int y) {
        if (x == cx && y == cy) return false; // ignores the first point so we can draw walls as well
        return map.getTile(x, y) == Tile::Tree || map.getTile(x, y) == Tile::Tree1 || map.getTile(x, y) == Tile::Tree2;
    };

    for (cx = dx; cx < dx + ScreenSize.x; cx++) {
        for (cy = dy; cy < dy + ScreenSize.y; cy++) {
            if (fov.LOS(sourcePosInTiles.x, sourcePosInTiles.y, cx, cy, checkblock))
                this->setShadow(cx, cy, 2);

            sf::Color buff;
            switch (shadowValue[cy*world.x + cx])
            {
            case 0:
                //buff = sf::Color(0x3B, 0x5C, 0x27,255);
                //buff = sf::Color(0x30, 0x50, 0x20, 255);
                buff = sf::Color(30, 30, 30, 255);
                break;
            case 1:
                buff = sf::Color(0, 0, 0, 125);
                break;
            case 2:
                buff = sf::Color(0, 0, 0, 0);
                break;
            default:
                buff = sf::Color(70, 70, 70, 255);
                break;
            }
            sf::Vertex* quad = &vertices[(cx-dx + (cy-dy) * ScreenSize.x) * 4];

            quad[0].position = sf::Vector2f((cx - dx + 0.f) * tileSize.x, (cy - dy + 0.f) * tileSize.y);
            quad[1].position = sf::Vector2f((cx - dx + 1.f) * tileSize.x, (cy - dy + 0.f) * tileSize.y);
            quad[2].position = sf::Vector2f((cx - dx + 1.f) * tileSize.x, (cy - dy + 1.f) * tileSize.y);
            quad[3].position = sf::Vector2f((cx - dx + 0.f) * tileSize.x, (cy - dy + 1.f) * tileSize.y);


            if (buff != sf::Color(0, 0, 0, 255))
            {
                quad[0].color = buff;
                quad[1].color = buff;
                quad[2].color = buff;
                quad[3].color = buff;
            }
            else
            {
                //quad[0].texCoords = sf::Vector2f((4 + 0.f) * 36, (1 + 0.f) * 36);
                //quad[1].texCoords = sf::Vector2f((4 + 1.f) * 36, (1 + 0.f) * 36);
                //quad[2].texCoords = sf::Vector2f((4 + 1.f) * 36, (1 + 1.f) * 36);
                //quad[3].texCoords = sf::Vector2f((4 + 0.f) * 36, (1 + 1.f) * 36);
            }

        }
    }

}


void Lightmap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &tileset;
    states.transform *= getTransform();
    target.draw(&vertices[0], vertices.size(), sf::Quads, states);
}


