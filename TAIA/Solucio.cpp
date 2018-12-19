#include "Solucio.h"
#include <ctime>


Solucio::Solucio()
{
	_hiHaCarril.resize(_arestes->size(),false);
	srand(time(NULL));
	while(_dist<_maxDist)
	{
		int indexRand = rand()%_arestes->size();
		if(!_hiHaCarril[indexRand])
		{
			_hiHaCarril[indexRand]=true;
			_dist+=(*_arestes)[indexRand].getLength();
		}
	}
}


Solucio::~Solucio()
{
}

double Solucio::getFitness()
{
	srand(0);
	int distTotal=0;
	int distCarril=0;
	int interesTotal=0;
	//Obtenim interes total
	for(auto&[uuid, n] : *_nodes)
	{
		interesTotal+=n.getInteres();
	}
	//Per cada habitant, visitem un lloc d interes aleatori(o no)
	for(auto&[uuid, n] : *_nodes)
	{
		int puntInteres = rand()%interesTotal;
		for(auto&[uuid2, n2] : *_nodes)
		{
			puntInteres-=n2.getInteres();
			if(puntInteres<=0)
			{
				auto res = getShortestPath(uuid,uuid2);
				distTotal+=res.first;
				distCarril+=res.second;				
			}
		}
	}
	return (double)((double)distCarril/ (double)distTotal);

}

void Solucio::setup(std::map<long, Node>* n, std::vector<Aresta>* a, int d)
{
	_arestes=a;
	_nodes=n;
	_distTotalPossible=d;
	_maxDist=0.33f*d;
	/*int nombreNodes=0;
	for(auto&[uuid,n] : *_nodes)
	{
		nombreNodes++;
	}
	int debug = _nodes->size();
	for(int i =0;i<nombreNodes;++i){
		_pathsFacils.push_back(std::vector<bool>(nombreNodes,false));
	}
	for(int )*/

}

std::pair<int, int> Solucio::getShortestPath(int uuid1, int uuid2) //implementacio basica amb un flood fill
{
}
