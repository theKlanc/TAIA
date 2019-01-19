#pragma once
#include <vector>
#include "Aresta.hpp"

class Solucio
{
public:
	Solucio();
	~Solucio();
	double getFitness();
	Solucio mutate();
	Solucio fug(Solucio grill);
	bool operator<(Solucio& l);
	static bool comparacioSort(Solucio& l,Solucio& r);
	bool hiHaCarril(int n);
	static void setup(std::map<long, Node>*n,std::vector<Aresta>*a,int d);
	
	int getDist();
	int getMaxDist();
	int getNTrams();
private:

	int _dist=0;
	bool dirtyFlag = true;
	double fitness;
	std::vector<bool> _hiHaCarril;
	//std::pair<int,int> getShortestPath(long uuid1, long uuid2);
	std::vector<int> getShortestPath(long uuid1, long uuid2);
	std::vector<Aresta*> getPathsFromNode(long uuid);

	//static std::vector<std::vector<bool>> _pathsFacils;
	Solucio reemplenar();

	static int _maxDist;
	static int _distTotalPossible;
	static std::map<long, Node> *_nodes;
	static std::vector<Aresta> *_arestes;
	static std::vector<std::vector<int>> _path;
};

