#pragma once


#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string>
#include <memory>
#include <vector>

class HUD : public sf::Drawable, public sf::Transformable
{
public:
	HUD(sf::Vector2f ScreenSize, sf::Font& font);

	void load(int hp, int maxHp, int heroLvl, int xp, int needXp, int poisonTime, int confuseTime, std::string enemyType, int enemyLvl, int enemyHp, int enemyMaxHp);
private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Vector2f screenSize;
	std::vector<sf::Vertex> vertices;
	sf::Text hp, xp, enemyInfo;
	sf::Font font;
};