#include "HUD.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <string>
#include <time.h>
#include <iostream>

HUD::HUD(sf::Vector2f screenSize, sf::Font& font) :
	screenSize(screenSize),
	font(font)
{
	hp.setFont(font);
	hp.setCharacterSize(24);
	hp.setOutlineThickness(1.f);

	xp.setFont(font);
	xp.setCharacterSize(24);
	xp.setOutlineThickness(1.f);

	enemyInfo.setFont(font);
	enemyInfo.setCharacterSize(24);
	enemyInfo.setOutlineThickness(1.f);

}

void HUD::load(int hp, int maxHp, int heroLvl, int xp, int needXp, int poisonTime, int confuseTime, std::string enemyType, int enemyLvl, int enemyHp, int enemyMaxHp)
{
	HUD::xp.setString(sf::String("LvL: " + std::to_string(heroLvl) + '(' + std::to_string(xp) + "/" + std::to_string(needXp) + ')'));
	HUD::hp.setString(sf::String(std::to_string(hp) + "/" + std::to_string(maxHp)));
	if (enemyType != "")
	{
		std::string tmp = "";
		tmp = std::to_string(enemyLvl);
		HUD::enemyInfo.setString(sf::String(enemyType + " Level: " + tmp + " Hp: " + std::to_string(enemyHp) + '/' + std::to_string(enemyMaxHp)));
	}
	HUD::xp.setPosition(screenSize.x * 0.9 - HUD::xp.getString().getSize() * 6 - 18.0f, screenSize.y - 52.0f);
	HUD::hp.setPosition(screenSize.x * 0.1 - HUD::hp.getString().getSize() * 6 + 18.0f, screenSize.y - 52.0f);
	HUD::enemyInfo.setPosition(screenSize.x * 0.5 - HUD::enemyInfo.getString().getSize()*6, screenSize.y - 52.0f);

	vertices.clear();
	vertices.resize(16);
	vertices[0].position = sf::Vector2f(18.0f, screenSize.y - 54.0f);
	vertices[1].position = sf::Vector2f(18.0f, screenSize.y - 18.0f);
	vertices[2].position = sf::Vector2f(screenSize.x * 0.2 + 18.0f, screenSize.y - 18.0f);
	vertices[3].position = sf::Vector2f(screenSize.x * 0.2 + 18.0f, screenSize.y - 54.0f);
	vertices[4].position = sf::Vector2f(screenSize.x - 18.0f, screenSize.y - 54.0f);
	vertices[5].position = sf::Vector2f(screenSize.x - 18.0f, screenSize.y - 18.0f);
	vertices[6].position = sf::Vector2f(screenSize.x * 0.8 - 18.0f, screenSize.y - 18.0f);
	vertices[7].position = sf::Vector2f(screenSize.x * 0.8 - 18.0f, screenSize.y - 54.0f);
	for (int i = 0; i < 8; i++)
		vertices[i].color = sf::Color(0, 0, 0, 255);

	vertices[8].position = sf::Vector2f(20.0f, screenSize.y - 52.0f);
	vertices[9].position = sf::Vector2f(20.0f, screenSize.y - 20.0f);
	vertices[10].position = sf::Vector2f((screenSize.x * 0.2) * ((float)hp / (float)maxHp) + 16.0f, screenSize.y - 20.0f);
	vertices[11].position = sf::Vector2f((screenSize.x * 0.2) * ((float)hp / (float)maxHp) + 16.0f, screenSize.y - 52.0f);
	for (int i = 8; i < 12; i++)
		vertices[i].color = sf::Color(230, 10, 15, 255);

	vertices[12].position = sf::Vector2f(screenSize.x * 0.8 + screenSize.x * 0.2 * ((float)xp / (float)needXp) - 20.0f, screenSize.y - 52.0f);
	vertices[13].position = sf::Vector2f(screenSize.x * 0.8 + screenSize.x * 0.2 * ((float)xp / (float)needXp) - 20.0f, screenSize.y - 20.0f);
	vertices[14].position = sf::Vector2f(screenSize.x * 0.8 - 16.0f, screenSize.y - 20.0f);
	vertices[15].position = sf::Vector2f(screenSize.x * 0.8 - 16.0f, screenSize.y - 52.0f);
	for (int i = 12; i < 16; i++)
		vertices[i].color = sf::Color(0x75, 0xAF, 0x29, 255);
	
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(&vertices[0], vertices.size(), sf::Quads, states);
	target.draw(xp);
	target.draw(hp);
	target.draw(enemyInfo);
}
