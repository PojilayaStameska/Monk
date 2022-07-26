#include "Utility.hpp"
#include "MoveHelper.hpp"

#include <algorithm>
#include <string>
#include <list>
#include <iostream>
#include <ctime>

MoveHelper::MoveHelper(sf::Vector2i screenSize)
{
	nodes = new sNode[screenSize.x * screenSize.y];
}

MoveHelper::~MoveHelper()
{

}

std::vector<sf::Vector2i> MoveHelper::getMoveList(CollisionMap& collisionMap, int x_start, int y_start, int x_end, int y_end, int dx, int dy, sf::Vector2i screenSize)
{
	std::vector<sf::Vector2i> result;
	nodeStart = &nodes[y_start * screenSize.x + x_start];
	nodeEnd = &nodes[y_end * screenSize.x + x_end];
	for (int x = 0; x < screenSize.x; x++)
		for (int y = 0; y < screenSize.y; y++)
		{
			nodes[y * screenSize.x + x].x = x;
			nodes[y * screenSize.x + x].y = y;
			nodes[y * screenSize.x + x].bObstacle = collisionMap.getColl(dx + x, dy + y);
			nodes[y * screenSize.x + x].child = nullptr;
			nodes[y * screenSize.x + x].parent = nullptr;
			nodes[y * screenSize.x + x].bVisited = false;
		}
	nodeEnd->bObstacle = false;
	for (int x = 0; x < screenSize.x; x++)
		for (int y = 0; y < screenSize.y; y++)
		{
			if (y > 0)
				nodes[y * screenSize.x + x].vecNeighbours.push_back(&nodes[(y - 1) * screenSize.x + (x + 0)]);
			if (y < screenSize.y - 1)
				nodes[y * screenSize.x + x].vecNeighbours.push_back(&nodes[(y + 1) * screenSize.x + (x + 0)]);
			if (x > 0)
				nodes[y * screenSize.x + x].vecNeighbours.push_back(&nodes[(y + 0) * screenSize.x + (x - 1)]);
			if (x < screenSize.x - 1)
				nodes[y * screenSize.x + x].vecNeighbours.push_back(&nodes[(y + 0) * screenSize.x + (x + 1)]);

			if (y > 0 && x > 0)
				nodes[y * screenSize.x + x].vecNeighbours.push_back(&nodes[(y - 1) * screenSize.x + (x - 1)]);
			if (y < screenSize.y - 1 && x>0)
				nodes[y * screenSize.x + x].vecNeighbours.push_back(&nodes[(y + 1) * screenSize.x + (x - 1)]);
			if (y > 0 && x < screenSize.x - 1)
				nodes[y * screenSize.x + x].vecNeighbours.push_back(&nodes[(y - 1) * screenSize.x + (x + 1)]);
			if (y < screenSize.y - 1 && x < screenSize.x - 1)
				nodes[y * screenSize.x + x].vecNeighbours.push_back(&nodes[(y + 1) * screenSize.x + (x + 1)]);
		}

	for (int x = 0; x < screenSize.x; x++)
		for (int y = 0; y < screenSize.y; y++)
		{
			nodes[y * screenSize.x + x].bVisited = false;
			nodes[y * screenSize.x + x].fGlobalGoal = INFINITY;
			nodes[y * screenSize.x + x].fLocalGoal = INFINITY;
			nodes[y * screenSize.x + x].child = nullptr;
			nodes[y * screenSize.x + x].parent = nullptr;
		}

	auto distance = [](sNode* a, sNode* b)
	{
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	};

	auto heuristic = [distance](sNode* a, sNode* b)
	{
		return distance(a, b);
	};


	sNode* nodeCurrent = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);


	std::list<sNode*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);


	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)
	{
		listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front();

		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true;

		for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
		{

			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
				listNotTestedNodes.push_back(nodeNeighbour);

			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;
				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
			}
		}
	}
	if (nodeEnd != nullptr)
	{
		sNode* p = nodeEnd;
		while (p->parent != nullptr)
		{
			sf::Vector2i buff;
			p->parent->child = p;
			buff = sf::Vector2i(p->x -  p->parent->x, p->y - p->parent->y);
			p = p->parent;
			result.push_back(buff);
		}
	}
	return result;
}






