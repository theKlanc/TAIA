#include "Solucio.h"
#include <ctime>
#include <queue>
#include <map>
#include <iostream>
#include <vector>

int Solucio::_maxDist;
int Solucio::_distTotalPossible;
std::map<long, Node>* Solucio::_nodes;
std::vector<Aresta>* Solucio::_arestes;
std::vector<std::vector<int>> Solucio::_path;

Solucio::Solucio()
{
	_hiHaCarril.resize(_arestes->size(), false);
	while (_dist < _maxDist)
	{
		int indexRand = rand() % _arestes->size();
		if (!_hiHaCarril[indexRand])
		{
			_hiHaCarril[indexRand] = true;
			_dist += (*_arestes)[indexRand].getLength();
		}
	}
}


Solucio::~Solucio()
{
}

double Solucio::getFitness()
{
	double resultat = 0;
	if (!dirtyFlag)
		return fitness;
	
	int interesTotal = 0;
	//Obtenim interes total
	if (_path.size() == 0) //creem el path en cas k no estigui ja creat, nomes caldra fer ho un sol cop
	{
		srand(0);
		for (auto&[uuid, n] : *_nodes)
		{
			interesTotal += n.getInteres();
		}
		//Per cada habitant, visitem un lloc d interes aleatori(o no)
		for (auto&[uuid, n] : *_nodes)
		{
			int puntInteres = rand() % interesTotal;
			for (auto&[uuid2, n2] : *_nodes)
			{
				puntInteres -= n2.getInteres();
				if (puntInteres <= 0)
				{
					auto res = getShortestPath(uuid, uuid2);
					_path.push_back(res);
					break;
				}
			}
		}
	}
	{//Calculem les distancies
		for (auto vectorArestes : _path)
		{
			bool estemACarril = false;
			int nTrams = 0;
			int tramCarril = 0;
			int tramTotal = 0;
			for (int i : vectorArestes)
			{
				int len = (*_arestes)[i].getLength();
				tramTotal += len;
				if (_hiHaCarril[i])
				{
					tramCarril += len;
					if (!estemACarril)
					{
						estemACarril = true;
						nTrams++;
					}
				}
				else
				{
					estemACarril = false;
				}
			}
			if (nTrams > 0)
				resultat += (((double)(tramCarril)) / ((double)(nTrams)));
			//resultat+=((tramCarril/tramTotal)/nTrams)			
		}
	}
	dirtyFlag = false;
	fitness = resultat;
	return resultat;

}

Solucio Solucio::mutate()
{
	Solucio resultat(*this);
	dirtyFlag = true;
	int tramsBuits = 0;
	for (bool t : resultat._hiHaCarril)
	{
		if (!t)
			tramsBuits++;
	}
	int contador = 0;
	int nouTram = (rand() % tramsBuits) + 1;
	for (int i = 0; i < resultat._hiHaCarril.size(); i++)
	{
		if (!resultat._hiHaCarril[i]) {
			contador++;
			if (contador == nouTram)
			{
				resultat._hiHaCarril[i] = true;
				resultat._dist += (*resultat._arestes)[i].getLength();
				break;
			}
		}
	}
	return resultat.reemplenar();
}

Solucio Solucio::fug(Solucio grill)
{
	dirtyFlag = true;
	Solucio resultat = Solucio(*this);
	for (int i = 0; i < _hiHaCarril.size(); i++)
	{
		resultat._hiHaCarril[i] = (rand() % 2 == 0 ? *this : grill)._hiHaCarril[i];
	}
	return resultat.reemplenar();
}

bool Solucio::operator<( Solucio& l)
{
	return getFitness() < l.getFitness();
}

bool Solucio::comparacioSort( Solucio& l,  Solucio& r)
{
	return l.getFitness() > r.getFitness();
}

bool Solucio::hiHaCarril(int n)
{
	return _hiHaCarril[n];
}

void Solucio::setup(std::map<long, Node>* n, std::vector<Aresta>* a, int d)
{
	_arestes = a;
	_nodes = n;
	_distTotalPossible = d;
	_maxDist = 0.3f*d;
	_path;
}

std::vector<int> Solucio::getShortestPath(long uuid1, long uuid2)
{
	//std::map<long,int> cami;
	//std::map<long,int> carril;
	std::map<long, long> antecessors;
	std::map<long, bool> visitats;
	std::queue<long> cua;
	std::map<long, int> camiPerArribar;

	if (uuid1 == uuid2)
	{
		return std::vector<int>();
	}
	bool trobat = false;
	long nodeActual = uuid1;
	do
	{
		auto temp = getPathsFromNode(nodeActual);
		for (auto& aresta : temp)
		{
			if (visitats.find(aresta->getDestination()) != visitats.end())//si ja l hem visitat
			{
				//no fer res
			}
			else
			{
				int index;
				for (int i = 0; i < _arestes->size(); i++)
				{
					if ((*_arestes)[i].getOrigin() == aresta->getOrigin() && (*_arestes)[i].getDestination() == aresta->getDestination())
					{
						index = i;

						break;
					}
				}
				antecessors.insert(std::make_pair(aresta->getDestination(), nodeActual));
				camiPerArribar.insert(std::make_pair(aresta->getDestination(), index));
				visitats.insert(std::make_pair(aresta->getDestination(), true));
				cua.push(aresta->getDestination());
			}
		}
		if (cua.empty())
		{
			continue;
		}
		nodeActual = cua.front();
		if (nodeActual == uuid2) { trobat = true; }
		cua.pop();
	} while (!trobat);
	if (!trobat)
		throw "ERROR BINCHE BUTO";
	bool done = false;
	long seguent = uuid2;
	std::vector<int> resultat;
	while (seguent != uuid1)
	{
		seguent = antecessors[seguent];
		resultat.push_back(camiPerArribar[seguent]);
	}
	return resultat;
}

std::vector<Aresta*> Solucio::getPathsFromNode(long uuid)
{
	std::vector<Aresta*> resultat;
	for (auto& temp : *_arestes)
	{
		if (temp.getOrigin() == uuid)
			resultat.push_back(&temp);
	}
	return resultat;
}

int Solucio::getDist()
{
	return _dist;
}

int Solucio::getMaxDist()
{
	return _maxDist;
}

int Solucio::getNTrams()
{
	int n=0;
	for(bool b : _hiHaCarril)
	{
		if(b)n++;
	}
	return n;
}

Solucio Solucio::reemplenar()
{
	Solucio resultat(*this);
	dirtyFlag = true;
	_dist=0;
	for(int i =0;i<_hiHaCarril.size();i++)
	{
		if(_hiHaCarril[i])
			_dist+=(*_arestes)[i].getLength();
	}
	while (resultat._dist < resultat._maxDist) {
		int tramsBuits = 0;
		for (bool t : resultat._hiHaCarril)
		{
			if (!t)
				tramsBuits++;
		}
		int contador = 0;
		int nouTram = (rand() % tramsBuits);
		for (int i = 0; i < resultat._hiHaCarril.size(); i++)
		{
			if (!resultat._hiHaCarril[i]) {
				if (contador++ == nouTram)
				{
					resultat._hiHaCarril[i] = true;
					resultat._dist += (*resultat._arestes)[i].getLength();
					break;
				}
			}
		}
	}
	while (resultat._dist > resultat._maxDist)
	{
		int tramsPlens = 0;
		for (bool t : resultat._hiHaCarril)
		{
			if (t)
				tramsPlens++;
		}
		int contador = 0;
		int tramEsborrar = (rand() % tramsPlens);
		for (int i = 0; i < resultat._hiHaCarril.size(); i++)
		{
			if (resultat._hiHaCarril[i]) {
				if (contador++ == tramEsborrar)
				{
					resultat._dist -= (*resultat._arestes)[i].getLength();
					resultat._hiHaCarril[i] = false;
					break;
				}
			}
		}
	}
	return resultat;
}
