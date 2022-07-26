#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>

class Map;

class Tilemap : public sf::Drawable, public sf::Transformable
{
public:
	Tilemap(const sf::Texture& tileset, const sf::Vector2i& tileSize, sf::Vector2i screenSize);

	void load(Map& map, int dx, int dy);

private:

	int getTileNumber(Map& map, int x, int y);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const sf::Texture& tileset;
	const sf::Vector2i tileSize;
	const sf::Vector2i screenSize;
	std::vector<sf::Vertex> vertices;
};
