#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>

class Map;

class Lightmap : public sf::Drawable, public sf::Transformable
{
public:
	Lightmap(const sf::Texture& tileset, const sf::Vector2i actionSize, sf::Vector2i tileSize);

	void clear();

	void setShadow(int x, int y, int value);

	void makeSomeLight(const Map& map, int dx, int dy, sf::Vector2i sourcePosInTiles, sf::Vector2f ScreenSize);

	int getShadow(int x, int y);

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Vector2i world;
	sf::Vector2i tileSize;
	const sf::Texture& tileset;
	std::vector<sf::Vertex> vertices;
	std::vector<int> shadowValue;
};
