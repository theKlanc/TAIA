#include "Solucio.h"
#include <ctime>
#include <queue>
#include <map>
#include <iostream>

int Solucio::_maxDist;
int Solucio::_distTotalPossible;
std::map<long, Node>* Solucio::_nodes;
std::vector<Aresta>* Solucio::_arestes;

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
	if(!dirtyFlag)
		return fitness;
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
				break;
			}
		}
	}
	dirtyFlag=false;
	return (double)((double)distCarril/ (double)distTotal);

}

bool Solucio::operator<(Solucio& l)
{
	return getFitness()<l.getFitness();
}

void Solucio::setup(std::map<long, Node>* n, std::vector<Aresta>* a, int d)
{
	_arestes=a;
	_nodes=n;
	_distTotalPossible=d;
	_maxDist=0.33f*d;
}

std::pair<int, int> Solucio::getShortestPath(long uuid1, long uuid2) //implementacio basica amb un flood fill
{
	std::map<long,int> cami;
	std::map<long,int> carril;
	std::map<long,long> antecessors;
	std::map<long,bool> visitats;
	std::queue<long> cua;

	if(uuid1==uuid2)
	{
		return std::make_pair(0,0);
	}
	bool trobat=false;
	long nodeActual = uuid1;
	do
	{
		auto temp = getPathsFromNode(nodeActual);
		for(auto& aresta : temp)
		{
			if(visitats.find(aresta->getDestination())!=visitats.end())//si ja l hem visitat
			{
				//no fer res
			}
			else
			{
				int index;
				for(int i =0;i<_arestes->size();i++)
				{
					if((*_arestes)[i].getOrigin()==aresta->getOrigin()&&(*_arestes)[i].getDestination()==aresta->getDestination())
					{
						index=i;
						break;
					}
				}
				cami.insert(std::make_pair(aresta->getDestination(),aresta->getLength()));
				carril.insert(std::make_pair(aresta->getDestination(),_hiHaCarril[index]?aresta->getLength():0));
				antecessors.insert(std::make_pair(aresta->getDestination(),nodeActual));
				visitats.insert(std::make_pair(aresta->getDestination(),true));
				cua.push(aresta->getDestination());
			}
		}
		if(cua.empty())
		{
			continue;
		}
		nodeActual=cua.front();
		if(nodeActual==uuid2){trobat=true;}
		cua.pop();
	}while(!trobat);
	if(!trobat)
		throw "ERROR BINCHE BUTO";
	int distCami=0;
	int distCarril=0;
	bool done = false;
	long seguent = uuid2;
	while(seguent != uuid1)
	{
		distCami+=cami[seguent];
		distCarril+=carril[seguent];
		seguent=antecessors[seguent];
	}
	return std::make_pair(distCami,distCarril);
}

std::vector<Aresta*> Solucio::getPathsFromNode(long uuid)
{
	std::vector<Aresta*> resultat;
	for(auto& temp : *_arestes)
	{
		if(temp.getOrigin()==uuid)
			resultat.push_back(&temp);
	}
	return resultat;
}
