#pragma once
#include <vector>
#include "Aresta.hpp"

class Solucio
{
public:
	Solucio();
	~Solucio();
	double getFitness();
	void mutate();
	Solucio fug(Solucio grill);
	bool operator<(Solucio& l);

	static void setup(std::map<long, Node>*n,std::vector<Aresta>*a,int d);

private:

	int _dist=0;
	bool dirtyFlag = true;
	double fitness;
	std::vector<bool> _hiHaCarril;
	std::pair<int,int> getShortestPath(long uuid1, long uuid2);
	std::vector<Aresta*> getPathsFromNode(long uuid);

	//static std::vector<std::vector<bool>> _pathsFacils;

	static int _maxDist;
	static int _distTotalPossible;
	static std::map<long, Node> *_nodes;
	static std::vector<Aresta> *_arestes;
};

