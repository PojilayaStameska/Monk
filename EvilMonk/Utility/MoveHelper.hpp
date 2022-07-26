#pragma once
#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "../Application/CollisionMap.hpp"

class Map;

class MoveHelper
{
public:


	MoveHelper(sf::Vector2i screenSize);
	~MoveHelper();

	std::vector<sf::Vector2i> getMoveList(CollisionMap& collisionMap, int x_start, int y_start, int x_end, int y_end, int dx, int dy, sf::Vector2i screenSize);

private:
	struct sNode
	{
		bool bObstacle = false;			// Is the node an obstruction
		bool bVisited = false;			// Have we searched this node before?
		float fGlobalGoal;				// Distance to goal so far
		float fLocalGoal;				// Distance to goal if we took the alternative route
		int x;							// Nodes position in 2D space
		int y;
		std::vector<sNode*> vecNeighbours;	// Connections to neighbours
		sNode* parent;					// Node connecting to this node that offers shortest parent
		sNode* child;
	};
	sNode* nodes = nullptr;
	sNode* nodeStart = nullptr;
	sNode* nodeEnd = nullptr;
};