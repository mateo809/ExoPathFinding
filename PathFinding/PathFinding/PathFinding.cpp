#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>

struct Node
{
	int X;
	int Y;
	int GWeight;
	bool bIsObstacle;
	Node* Parent;

	Node(int _X = 0, int _Y = 0) : X(_X), Y(_Y), GWeight(0), bIsObstacle(false), Parent(nullptr) {}
};

class Pathfinding
{
public:
	std::vector<Node*> FindPath(std::vector<std::vector<Node>>& Graph, int SizeX, int SizeY, Node* Start, Node* End)
	{
		std::vector<Node*> ClosedList;
		std::vector<Node*> OpenList;
		bool TargetIsFound = false;

		Start->GWeight = 0;
		OpenList.push_back(Start);

		while (OpenList.size() != 0 && TargetIsFound != true)
		{
			Node* LeastCostNode = ExtractLeastCostNode(OpenList, End, SizeX, SizeY);

			if (LeastCostNode == nullptr) break;

			if (LeastCostNode->X == End->X && LeastCostNode->Y == End->Y)
			{
				TargetIsFound = true;
				End->Parent = LeastCostNode->Parent;
				break;
			}

			std::vector<Node*> Neighbors = GetNeighbors(Graph, LeastCostNode, SizeX, SizeY, ClosedList);

			for (Node* Neighbor : Neighbors)
			{
				if (Neighbor->X == End->X && Neighbor->Y == End->Y)
				{
					TargetIsFound = true;
					Neighbor->GWeight = LeastCostNode->GWeight + CalculateDistance(LeastCostNode, Neighbor);
					Neighbor->Parent = LeastCostNode;
					break;
				}

				int NewGWeight = LeastCostNode->GWeight + CalculateDistance(LeastCostNode, Neighbor);

				auto it = std::find(OpenList.begin(), OpenList.end(), Neighbor);
				if (it == OpenList.end())
				{
					Neighbor->GWeight = NewGWeight;
					Neighbor->Parent = LeastCostNode;
					OpenList.push_back(Neighbor);
				}
				else if (NewGWeight < Neighbor->GWeight)
				{
					Neighbor->GWeight = NewGWeight;
					Neighbor->Parent = LeastCostNode;
				}
			}

			ClosedList.push_back(LeastCostNode);
		}

		std::vector<Node*> Path;
		if (TargetIsFound)
		{
			Node* Current = End;
			while (Current != nullptr)
			{
				Path.insert(Path.begin(), Current);
				Current = Current->Parent;
			}
		}

		return Path;
	}
private:
	int CalculateDistance(Node* NodeA, Node* NodeB)
	{
		return abs(NodeA->X - NodeB->X) + abs(NodeA->Y - NodeB->Y);
	}

	Node* ExtractLeastCostNode(std::vector<Node*>& OpenList, Node* Target, int SizeX, int SizeY)
	{
		Node* LeastCostNode = nullptr;
		int LeastCostNodeWeight = INT_MAX;
		int LeastCostNodeIndex = -1;

		for (int Index = 0; Index < OpenList.size(); Index++)
		{
			int CoordWeight = OpenList[Index]->GWeight + CalculateDistance(OpenList[Index], Target);

			if (CoordWeight < LeastCostNodeWeight)
			{
				LeastCostNode = OpenList[Index];
				LeastCostNodeWeight = CoordWeight;
				LeastCostNodeIndex = Index;
			}
		}

		if (LeastCostNodeIndex != -1)
		{
			OpenList.erase(OpenList.begin() + LeastCostNodeIndex);
		}

		return LeastCostNode;
	}

	std::vector<Node*> GetNeighbors(std::vector<std::vector<Node>>& Graph, Node* LeastCostNode, int SizeX, int SizeY, std::vector<Node*>& ClosedList)
	{
		std::vector<Node*> Neighbors;

		for (int X = -1; X <= 1; X++)
		{
			for (int Y = -1; Y <= 1; Y++)
			{
				if (X == 0 && Y == 0)
				{
					continue;
				}

				int NeighborX = LeastCostNode->X + X;
				int NeighborY = LeastCostNode->Y + Y;

				if (NeighborX < 0 || NeighborX >= SizeX || NeighborY < 0 || NeighborY >= SizeY)
				{
					continue;
				}

				Node* Neighbor = &Graph[NeighborX][NeighborY];

				if (Neighbor->bIsObstacle)
				{
					continue;
				}

				if (std::find(ClosedList.begin(), ClosedList.end(), Neighbor) != ClosedList.end())
				{
					continue;
				}

				Neighbors.push_back(Neighbor);
			}
		}

		return Neighbors;
	}
};

int main()
{
	int SizeX = 100;
	int SizeY = 100;

	std::vector<std::vector<Node>> Graph(SizeX, std::vector<Node>(SizeY));

	for (int x = 0; x < SizeX; x++)
	{
		for (int y = 0; y < SizeY; y++)
		{
			Graph[x][y] = Node(x, y);
		}
	}

	for (int x = 0; x < SizeX; x++)
	{
		for (int y = 0; y < SizeY; y++)
		{
			if (rand() % 100 < 20)
			{
				Graph[x][y].bIsObstacle = true;
			}
		}
	}

	Graph[0][0].bIsObstacle = false;
	Graph[99][99].bIsObstacle = false;

	Node* Start = &Graph[0][0];
	Node* End = &Graph[99][99];

	Pathfinding pf;
	std::vector<Node*> Path = pf.FindPath(Graph, SizeX, SizeY, Start, End);

	std::cout << std::endl << std::endl;

	//Couleurs
	const std::string RED = "\033[31m";
	const std::string BLUE = "\033[34m";
	const std::string GREEN = "\033[32m";
	const std::string RESET = "\033[0m";

	std::cout << "(100x100):" << std::endl;
	for (int y = 0; y < SizeY; y++)
	{
		for (int x = 0; x < SizeX; x++)
		{
			if (x % 2 != 0) continue;

			Node* CurrentNode = &Graph[x][y];
			bool InPath = std::find(Path.begin(), Path.end(), CurrentNode) != Path.end();

			if (CurrentNode->bIsObstacle)
			{
				std::cout << GREEN << "#" << RESET;
			}
			else if (InPath)
			{
				std::cout << RED << "1" << RESET;
			}
			else
			{
				std::cout << BLUE << "0" << RESET;
			}
		}
		std::cout << std::endl;
	}

	return 0;
}